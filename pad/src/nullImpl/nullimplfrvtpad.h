/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef NULLIMPLFRVTPAD_H_
#define NULLIMPLFRVTPAD_H_

#include "frvt_pad.h"

/*
 * Declare the implementation class of the FRVT PAD Interface
 */
namespace FRVT_PAD {
    class NullImplFRVTPad : public FRVT_PAD::Interface {
public:

    NullImplFRVTPad();
    ~NullImplFRVTPad() override;

    FRVT::ReturnStatus
    initialize(
        const std::string &configDir) override;

    FRVT::ReturnStatus
    detectImpersonationPA(
        const FRVT_PAD::Media &suspectedPA,
        bool &isPA,
        double &score,
        std::vector< std::pair<std::string, std::string> > &decisionProperties) override;

    FRVT::ReturnStatus
    detectEvasionPA(
        const FRVT_PAD::Media &suspectedPA,
        bool &isPA,
        double &score,
        std::vector< std::pair<std::string, std::string> > &decisionProperties) override;

    static std::shared_ptr<FRVT_PAD::Interface>
    getImplementation();

private:
    std::string configDir;
    // Some other members
};
}

#endif /* NULLIMPLFRVTPAD_H_ */
