# include pri file from ColoradoServiceGui/Build

!include("../../../Build/ColoradoService.pri") {
    error("../../../Build/ColoradoService.pri not found")
}
