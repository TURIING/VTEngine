//
// Created by turiing on 2025/1/7.
//

#ifndef RHIINSTANCEBUILDER_H
#define RHIINSTANCEBUILDER_H
#include "common/common.h"
#include "core/RHI/RHIInstance.h"


class RHIInstanceBuilder {
public:
    RHIInstanceBuilder() = default;
    RHIInstanceBuilder& SetEnableValidationLayer(bool on);
    RHIInstanceBuilder& SetInstanceExtensions(const std::vector<const char*>& extensions);
    RHIInstanceBuilder& SetLayer(const std::vector<const char*>& layers);
    std::shared_ptr<RHIInstance> Build();

private:
    RHIInstanceCreateInfo m_createInfo {};
};

#endif //RHIINSTANCEBUILDER_H
