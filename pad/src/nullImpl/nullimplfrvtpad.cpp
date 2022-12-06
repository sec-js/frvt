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
    double &score)
{
    if((rand() % 3) == 0) {
        isPA = true;
        score = rand()/(double)(RAND_MAX - 1);
    } else {
        isPA = false;
        score = -1.0 * (rand()/(double)(RAND_MAX - 1));
    }

    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVTPad::detectEvasionPA(
    const FRVT_PAD::Media &suspectedPA,
    bool &isPA,
    double &score)
{
    if((rand() % 5) == 0) {
        isPA = true;
        score = rand()/(double)(RAND_MAX - 1);
    } else {
        isPA = false;
        score = -1.0 * (rand()/(double)(RAND_MAX - 1));
    }

    return ReturnStatus(ReturnCode::Success);
}

std::shared_ptr<Interface>
Interface::getImplementation()
{
    return std::make_shared<NullImplFRVTPad>();
}

