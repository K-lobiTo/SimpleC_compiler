int global_var = 2;
int it_works = 1;

int main() {
    int x = 5;
    for(int i = 0; i < 3; i++){
        x = x+1;
    }
    while(x--){
        for(int j = 0; j<x; ++j){
            j+=x;
        }
    }
    if(x){

    }
}