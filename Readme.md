# DCPInterpreter
// 时光的细节

# UpdateList
* 2017.8.31
    * 寒江雪1719
        *  Create Interpreter.h
        *  After that,fix some bug.
        *  Change STRING_TOOL::IsNum function.It can ignore character that can't not been seen.(ASCII:0~32 and 127)
        *  Have fun~!!
* 2017.9.2
    * 寒江雪1719
        * Create FileCache.h
        * Alter class——AccountingInterpreter
        * Change output file format.All process result will ouput as middle file(.dcpm)
* 2017.9.3
    * 寒江雪1719
        * Create Excel.h
        * Add function OutputDCPMFileToExcel——Read DCPM file and output as excel format.
        * Add FILE_TOOL in Tool.h.
        * Add function FileExist.Try to check file exist or not.
