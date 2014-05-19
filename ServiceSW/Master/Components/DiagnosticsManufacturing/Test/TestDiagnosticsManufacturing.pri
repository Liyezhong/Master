# include pri file from Master/Test

!include("../../../Test/ColoradoService.pri") {
    error("../../../Test/ColoradoService.pri not found")
}
