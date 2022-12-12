/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <cstring>
#include <cstdlib>
#include <utility>

#include "nullimplfrvtpad.h"

using namespace std;
using namespace FRVT;
using namespace FRVT_PAD;

NullImplFRVTPad::NullImplFRVTPad() {}

NullImplFRVTPad::~NullImplFRVTPad() {}

ReturnStatus
NullImplFRVTPad::initialize(
    const std::string &configDir)
{
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVTPad::detectImpersonationPA(
    const FRVT_PAD::Media &suspectedPA,
    bool &isPA,
    double &score,
    std::vector< std::pair<std::string, std::string> > &decisionProperties)
{
    if ((rand() % 3) == 0) {
        isPA = true;
        score = rand()/(double)(RAND_MAX - 1);
    } else {
        isPA = false;
        score = -1.0 * (rand()/(double)(RAND_MAX - 1));
    }

    if (score > 0)
        decisionProperties.push_back(std::make_pair("PA detected", "replay attack"));
    else if (score > -0.09 && score < 0.09)
        decisionProperties.push_back(std::make_pair("unable to make PA decision", "image resolution too low"));
    
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVTPad::detectEvasionPA(
    const FRVT_PAD::Media &suspectedPA,
    bool &isPA,
    double &score,
    std::vector< std::pair<std::string, std::string> > &decisionProperties)
{
    if((rand() % 5) == 0) {
        isPA = true;
        score = rand()/(double)(RAND_MAX - 1);
    } else {
        isPA = false;
        score = -1.0 * (rand()/(double)(RAND_MAX - 1));
    }

    if (score > 0) {
        decisionProperties.push_back(std::make_pair("PA detected", "exaggerated expression"));
        decisionProperties.push_back(std::make_pair("PA detected", "eyes not visible"));
    } else if (score > -0.09 && score < 0.09)
        decisionProperties.push_back(std::make_pair("unable to make PA decision", "image resolution too low"));

    return ReturnStatus(ReturnCode::Success);
}

std::shared_ptr<Interface>
Interface::getImplementation()
{
    return std::make_shared<NullImplFRVTPad>();
}

