# include pri file from HimalayaServiceGui/Build

!include("../../../Build/HimalayaService.pri") {
    error("../../../Build/HimalayaService.pri not found")
}
