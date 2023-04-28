#!/bin/bash

source ../common/scripts/utils.sh

# Make sure there aren't any zombie processes
# left over from previous validation run
kill -9 $(ps -aef | grep "count_thread" | awk '{ print $2 }') 2> /dev/null

configDir=config
if [ ! -e "$configDir" ]; then
	echo "${bold}[ERROR] Missing ./$configDir folder!${normal}"
	exit $failure	
fi

outputDir=validation
rm -rf $outputDir; mkdir -p $outputDir

# Usage: ../bin/validate_pad detectImpersonationPA|detectEvasionPA -c configDir -o outputDir -h outputStem -i inputFile -t numForks
#
#   detectImpersonationPA|detectEvasionPA: task to process
#   configDir: configuration directory
#   outputDir: directory where output logs are written to
#   outputStem: the string to prefix the output filename(s) with
#   inputFile: input file containing images to process (required for enroll and verif template creation)
#   numForks: number of processes to fork

echo "------------------------------"
echo " Running FRVT PAD Validation"
echo "------------------------------"
for action in detectImpersonationPA detectEvasionPA
do
	# Set number of child processes to fork()
	numForks=1
	inputFile=input/short_pad.txt
	outputStem=$action

	echo -n "$action - checking for hard-coded config directory "
	tempConfigDir=otherConfig
	chmod 775 $configDir; mv $configDir $tempConfigDir; chmod 550 $tempConfigDir
	bin/validate_pad $action -c $tempConfigDir -o $outputDir -h $outputStem -i $inputFile -t $numForks
	ret=$?
	if [[ $ret == 0 ]]; then
		echo "[SUCCESS]" 
		# Merge output files together
		merge $outputDir/$outputStem log
    elif [[ $ret == 2 ]]; then
        echo "[NOT IMPLEMENTED]"
	else
		chmod 775 $tempConfigDir
		mv $tempConfigDir $configDir
		echo "[ERROR] Detection of hard-coded config directory in your software.  Please fix!"
		exit $failure
	fi
	rm -rf $outputDir/*
	chmod 775 $tempConfigDir; mv $tempConfigDir $configDir; chmod 550 $configDir

	inputFile=input/detectImpersonationPA.txt
	echo -n "$action - generating pad values (single process) "
	# Start checking for threading
	../common/scripts/count_threads.sh validate_pad $outputDir/thread.log & pid=$!

	bin/validate_pad $action -c $configDir -o $outputDir -h $outputStem -i $inputFile -t $numForks
	ret=$?

	# End checking for threading
	kill -9 "$pid"
	wait "$pid" 2>/dev/null

	if [[ $ret == 0 ]]; then
		echo "[SUCCESS]" 
		# Merge output files together
		merge $outputDir/$outputStem log

		maxThreads=$(cat $outputDir/thread.log | sort -u -n | tail -n1)
		# 1 process for testdriver, 1 process for child
		if [ "$maxThreads" -gt "2" ]; then
			echo "${bold}[WARNING] We've detected that your software may be threading or using other multiprocessing techniques.  The number of processes detected was $maxThreads and it should be 2.  Per the API document, implementations must run single-threaded.  In the test environment, there is no advantage to threading, because NIST will distribute workload across multiple blades and multiple processes.  We highly recommend that you fix this issue prior to submission.${normal}"
		fi
    elif [[ $ret == 2 ]]; then
        echo "[NOT IMPLEMENTED]"
	else
		echo "${bold}[ERROR] $action pad validation (single process) failed${normal}"
		exit $failure
	fi

	rm -rf $outputDir/*
done

for action in detectImpersonationPA detectEvasionPA 
do
	numForks=4
	inputFile=input/$action.txt
	outputStem=$action
	echo -n "$action - generating pad values (multiple processes) "
	bin/validate_pad $action -c $configDir -o $outputDir -h $outputStem -i $inputFile -t $numForks
	ret=$?
	if [[ $ret == 0 ]]; then
		echo "[SUCCESS]"
		# Merge output files together
		merge $outputDir/$outputStem log
    elif [[ $ret == 2 ]]; then
        echo "[NOT IMPLEMENTED]"
	else
		echo "${bold}[ERROR] $action pad validation (multiple process) failed.  Please ensure your software is compatible with fork(2).${normal}"
		exit $failure
	fi
done
