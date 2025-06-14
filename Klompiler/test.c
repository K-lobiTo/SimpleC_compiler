int global_var = 2;
int it_works = 1;
int test = -1;
char* st = "str";
const unsigned char* si = "ss";

int main() {
    while (1)test++,it_works--;
    if(-10 < 0){}
    int y = -test;
    int z = -(-5);
    int x = 0;
    for(int i = 1; i < 3; i++){
        x = x+=2;
        if(x){break;}
    }
    for(; ;){
        x = x+=2;
        if(x){break;}
    }
    while(x--){
        for(int j = 0; j<10; ++j){
            j+=2;
            continue;
        }
    }
    if (x){
    }
}