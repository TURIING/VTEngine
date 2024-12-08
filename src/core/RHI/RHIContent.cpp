/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/11/2 14:46
* @version: 1.0
* @description: 
********************************************************************************/

#include "RHIContent.h"

#include "RHIDevice.h"
#include "RHIInstance.h"

RHIContent::RHIContent(const QWidget *window) {
    m_pInstance = std::make_shared<RHIInstance>(reinterpret_cast<HWND>(window->winId()));
    m_pDevice = std::make_shared<RHIDevice>(m_pInstance);
}
