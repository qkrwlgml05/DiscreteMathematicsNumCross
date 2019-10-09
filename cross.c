#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fill_matrix (FILE* file, int *N, int *M, int input[][1000], int row[], int column[]);
void parse(int M, int N);

int main (int argc, char** argv) {
    char filename[100];
    if (argc < 2){
        printf("number cross file name : ");
        fscanf(stdin, "%s", filename);
    }else{
        strcpy(filename, argv[1]);
    }
    FILE* file = fopen(filename, "r");
    FILE * fp = fopen("formula", "w");
    int input[1000][1000];
    int row[1000];
    int column[1000];
    int N = 0, M = 0;
    int i = 0, j = 0;
    
    fill_matrix (file, &N, &M, input, row, column);

    for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(declare-const a%d%d Int)\n", i, j);

    // 모든 값의 범위 정해주기
    for (i = 1; i <= M; i++)
        for (j = 1; j <= N; j++)
			fprintf(fp, "(assert (or (= 0 a%d%d) (= a%d%d %d)))\n", i, j, i, j, input[i-1][j-1]);

    for (i = 0; i < M; i++){
        fprintf(fp, "(assert ( = ( +");
        for (j = 0; j < N; j++){
            fprintf(fp, " a%d%d", i+1, j+1);
        }
        fprintf(fp, ") %d))\n", row[i]);
    }

    for (i = 0; i < N; i++){
        fprintf(fp, "(assert ( = ( +");
        for (j = 0; j < M; j++){
            fprintf(fp, " a%d%d", j+1, i+1);
        }
        fprintf(fp, ") %d))\n", column[i]);
    }
    fprintf(fp, "(check-sat)\n(get-model)\n") ;

	fclose(fp) ;

	FILE * fin = popen("z3 formula", "r") ; //FIXME
    FILE * fps = fopen("solution", "w") ; //FIXME
	char buf[128] ;
	fscanf(fin, "%s %s", buf, buf) ;
	while (!feof(fin)) {
		fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
		fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
		fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
		fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
		fscanf(fin, "%s", buf) ; fprintf(fps, "%s ", buf) ;
	}
    fclose(fps);
	pclose(fin);
    fprintf(stdout, "\n\n---NUMBER CROSS---\n");
    parse(M, N);

    return 0;
}


void fill_matrix (FILE* file, int *N, int *M, int input[][1000], int row[], int column[]){
    char line[500];
    int num_char = sizeof(line)/sizeof(char);
    int i = 0, j = 0;
    while (fgets(line, num_char, file) != NULL){
        int k = 0;
        j = 0;
        while(line[k++] != '\0'){
            if (line[k-1]>='0' && line[k-1]<='9'){
                if (i == 0){
                    if (line[k-2]>='0' && line[k-2]<='9')
                        column[j-1] = column[j-1]*10+line[k-1]-'0';
                    else 
                        column[j++] = line[k-1]-'0';
                }
                else if (j >= (*N)){
                    row[i-1] = row[i-1]*10+line[k-1]-'0';
                }else {
                    input[i-1][j++]=line[k-1]-'0';
                }
            }
        }
        if ((i++)==0){
            (*N)=j;
        }
    }
    (*M)=i-2;
}


void parse(int M, int N){
    FILE* file = fopen("solution", "r");
    int i, j, k;
    int board[100][100];

    char b[128] = "";
    char s[128] = "";
    char t[128] = "";

    for (k = 0; k < M*N; k++){
        fscanf(file, "%s %s %s %s %s", b, s, b, b, t);

        i = (s[1] >- '0' && s[1] <= '9' && t[0] >= '0' && t[0] <= '9') ? s[1]-'0' : 0;
        j = (s[2] >= '0' && s[2] <= '9' && t[0] >= '0' && t[0] <= '9') ? s[2]-'0' : 0;
        board[i][j] = 1;
        if (t[0] != '0'){
            board[i][j]=0;
        }
    }
    if (i==0 || j==0){
        fprintf(stdout, "no solution\n");
    }
    else{
        for (i = 1; i <= M; i++){
            for (j = 1; j <= N; j++){
                if (board[i][j] < 10)
                    fprintf(stdout, " ");
                fprintf(stdout, "%d ", board[i][j]);
            }
            fprintf(stdout, "\n");
        }
    }
}
