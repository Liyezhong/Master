# include pri file from Master/Test

!include("../../../Test/HimalayaService.pri") {
    error("../../../Test/HimalayaService.pri not found")
}
