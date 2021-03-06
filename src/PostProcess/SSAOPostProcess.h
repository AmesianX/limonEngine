//
// Created by engin on 12.12.2018.
//

#ifndef LIMONENGINE_SSAOPOSTPROCESS_H
#define LIMONENGINE_SSAOPOSTPROCESS_H


#include <iostream>
#include "QuadRenderBase.h"

class SSAOPostProcess : public QuadRenderBase {
    uint32_t sampleCount;
    void generateAndSetSSAOKernels(uint32_t kernelSize) const ;
    void initializeProgram() override;

public:
    SSAOPostProcess(GLHelper* glHelper, uint32_t sampleCount) : QuadRenderBase(glHelper), sampleCount(sampleCount) {
        initializeProgram();
        if(sampleCount > 128) {
            std::cerr << "Maximum sample count for SSAO is 128, your input will be lowered to it. " << std::endl;
            this->sampleCount = 128;
        }
        generateAndSetSSAOKernels(this->sampleCount);
    }

    uint32_t getSampleCount() const {
        return sampleCount;
    }

    void setSampleCount(uint32_t sampleCount) {
        if(sampleCount > 128) {
            std::cerr << "Maximum sample count for SSAO is 128, your input will be lowered to it. " << std::endl;
            sampleCount = 128;
        }
        SSAOPostProcess::sampleCount = sampleCount;
        generateAndSetSSAOKernels(sampleCount);
    }

};


#endif //LIMONENGINE_SSAOPOSTPROCESS_H
