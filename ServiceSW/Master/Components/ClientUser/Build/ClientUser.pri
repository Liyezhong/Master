# include pri file from ColoradoServiceGui/Build

!include("../../../Build/ColoradoServiceGui.pri") {
    error("../../../Build/ColoradoServiceGui.pri not found")
}
