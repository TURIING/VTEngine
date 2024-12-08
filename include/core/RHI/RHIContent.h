/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/11/2 14:46
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHICONTENT_H
#define RHICONTENT_H

#include "common/common.h"

class QWidget;
class RHIDevice;
class RHIInstance;

class RHIContent {
public:
    explicit RHIContent(const QWidget *window);

private:
    std::shared_ptr<RHIDevice> m_pDevice = nullptr;
    std::shared_ptr<RHIInstance> m_pInstance = nullptr;
};



#endif //RHICONTENT_H
