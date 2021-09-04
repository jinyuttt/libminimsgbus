#include "pch.h"
#include "PtpFactory.h"
#include "PtPNet.h"
namespace libminimsgbus
{
    IPtPNet& PtpFactory::Create()
    {
        PtPNet tmp;
        IPtPNet& pt = tmp;
        return pt;
    }
    IPtPNet* PtpFactory::CreatePoint()
    {
        IPtPNet* pt = new PtPNet();
        return pt;
    }
}