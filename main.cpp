#include <iostream>
#include <xlsxwriter.h>

int main(){
    //just hello world, remove this later
    std::cout << "Hello World";


    
    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("example.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    // Write some data to cells
    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 0, 1, 123, NULL);
    worksheet_write_number(worksheet, 1, 0, 456, NULL);

    workbook_close(workbook);
    return 0;
}
