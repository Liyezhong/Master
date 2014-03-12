# include pri file from HimalayaService/Build

!include("../../../Build/HimalayaService.pri") {
    error("../../../Build/HimalayaService.pri not found")
}
