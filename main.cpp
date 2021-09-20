#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename){
    SymbolTable *st = new SymbolTable();
    try{
        st->run(filename);
    }
    catch (exception &e){
        cout << e.what();
    }
    delete st;
}

int main(int argc, char **argv){
	// đoạn code này làm chương trình chạy sai
    // if (argc < 2)
    //     return 1;
	

    test("test1.txt");
    return 0;
}