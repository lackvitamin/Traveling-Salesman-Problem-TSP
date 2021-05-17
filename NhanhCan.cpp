#include <stdio.h>
#include <stdlib.h>

#define _fileName "DuLieu.txt"
#define _fileName2 "B&BGrapth.txt"
#define size 10

int N;
int M;
int MIN = 1000;

int MaTran[size][size];
int bestProcess[size];		// Chu trình tốt nhất
int bestWay = MIN;			// Khởi tạo nên cho nó là một số vô cùng lớn
bool visited[size];         // Đánh dấu ghé qua hay chưa (true nếu chưa ghé qua)

void _readFile(int *n, int *m)
{
    int i, j;
    FILE *f;
    f = fopen(_fileName,"r");
    if (f==NULL)
    {
        printf("File Loi!");
        return;
    }
    fscanf(f,"%d%d",n,m);
    for (i = 0; i < *m; i++) for (j = 0; j < *n; j++) fscanf(f,"%d",&MaTran[i][j]);
    fclose(f);
}

// Copy giải pháp tạm thời vào biến tốt nhất hiện thành
void _update(int tempProcess[])
{
	for (int i=0; i<M; i++)
		bestProcess[i] = tempProcess[i];
	bestProcess[M] = tempProcess[0];
}

// Hàm tìm quãng đường ngắn nhất
// Tìm cột nhỏ nhất ở hàng i
int _findMin1(int i)
{
	int min = MIN;
	for (int k=0; k < M; k++)
		if (MaTran[i][k]<min && i != k)
			min = MaTran[i][k];
	return min;
}

// Hàm tìm quãng đường ngắn thứ 2
// Tìm cột nhỏ thứ 2 tại hàng i
int _findMin2(int i)
{
	int first = MIN, second = MIN;
	for (int j = 0; j < M; j++)
	{
		if (i == j)
			continue;   // Lệnh continue giống như break nhưng thay vì kết thúc vòng lặp thì lại chạy vòng lặp tiếp theo

		if (MaTran[i][j] <= first)
		{
			second = first;
			first = MaTran[i][j];
		}
		else if (MaTran[i][j] <= second && MaTran[i][j] != first)
			second = MaTran[i][j];
	}
	return second;
}

void _comeBack(int currWay, int level, int tempProcess[])
{
	// Kiểm tra xem có phải là thành phố cuối cùng không
	// Trở về nút xuất phát
	if (MaTran[tempProcess[level-1]][tempProcess[0]] != 0)
	{
		// tempWay chứa độ dài đường đi vừa qua
		int tempWay = currWay + MaTran[tempProcess[level-1]][tempProcess[0]];

		// Cập nhật kết quả nếu tempWay ngắn hơn
		if (tempWay < bestWay)
		{
			_update(tempProcess);
			bestWay = tempWay;
		}
	}
	return; // Backtrack
}

void _tsp(float Can_Duoi, int currWay, int level, int tempProcess[])
{
	// Khi ta đi hết các thành phố
	if (level == N)
	{
		_comeBack(currWay, level, tempProcess);
	}

	// Khi chưa đi hết các thành phố ta đệ quy xây dựng cây (Phân nhánh)
	for (int i = 0; i < N; i++)
	{
		// Xem xét thành phố tiếp theo nếu nó không giống nhau và chưa được đi qua
		if (MaTran[tempProcess[level-1]][i] != 0 && visited[i] == false)
		{
			float temp = Can_Duoi;
			currWay += MaTran[tempProcess[level-1]][i];
			
			// Cận dưới = Cạn dưới cũ - ((chi phí cạnh tối thiểu second là 1 + chi phí cạnh first tối thiểu là 2) / 2) + chi phí cạnh 1-2) + giá trị đườg đi tại 2 nút
			if (level == 1) Can_Duoi -= ((_findMin1(tempProcess[level-1]) + _findMin1(i))/2);
			else Can_Duoi -= ((_findMin2(tempProcess[level-1]) + _findMin1(i))/2);
			
			// (Can_Duoi + currWay) là cận dưới của một nút con mà ta đang xét
			// Nếu cận dưới hiện tại < bestWay, chúng ta cần khám phá thêm nút
			if (Can_Duoi + currWay < bestWay)
			{
				tempProcess[level] = i;		// Đặt thành phố i vào hướng đến của level
				visited[i] = true;			// Đánh dấu đã đi qua 

				// Khám phá thêm nút (Đệ quy)
				_tsp(Can_Duoi, currWay, level+1, tempProcess);
			}

			// Ngược lại, sau khi cắt tỉa thì trả lại currWay và Can_Duoi về ban đầu
			currWay -= MaTran[tempProcess[level-1]][i];
			Can_Duoi = temp;

			// Đồng thời hoàn lại nút đã truy cập
			for (int a = 0; a < N; a++) visited[a] = false;
			for (int j=0; j <= level-1; j++) visited[tempProcess[j]] = true;
		}
	}
}

// Hàm chọn đường đi ngắn nhất lý tưởng
void _khoiTao(int x)
{
	int tempProcess[M];
	float Can_Duoi = 0;

	tempProcess[0] = -1;
	visited[0] = false;

	// Tạo cận nút gốc
	for (int i = 0; i < N; i++) Can_Duoi += (_findMin1(i) + _findMin2(i));
    Can_Duoi = Can_Duoi/2;

	// Thành phố bắt đầu
	visited[x] = true;
	tempProcess[0] = x;

	// Gọi đến tsp, cho currWay = 0 và thành phố 1
	_tsp(Can_Duoi, 0, 1, tempProcess);
}

void _printMaTran()
{
    printf("Ma tran:\n");
    for (int i = 0; i < N; i++)
    {
        printf("\t");
        for (int j = 0; j < N; j++)
            printf("%d ",MaTran[i][j]);
        printf("\n");
    }
}

void _print()
{
    printf("Thu tu cac thanh pho: ");
    for (int i = 0; i <= N; i ++)
    {
        printf("%c", bestProcess[i]+65);
		if (i!=N) printf(" -> ");
    }
    printf("\nTong do dai quang duong: %d\n", bestWay);
}

void _printFile(int *n, int *m)
{
    int i, j;
    FILE *f;
    f = fopen(_fileName2,"w");
    for (i = 0; i < *n; i++) fprintf(f,"%c  %c  %d\n",bestProcess[i]+65, bestProcess[i+1]+65, MaTran[bestProcess[i]][bestProcess[i+1]]);
    fclose(f);
}

int main()
{
	system("cls");
	char x;
	_readFile(&N,&M);
	_printMaTran();
	printf("Nhap diem xuat phat (a,b,c,...):  ");
	scanf("%c",&x);
	if(x>=97) x-=32;
	_khoiTao(x-65); // Chữ cái thành số
	_print();
    _printFile(&N,&M);
	system("pause");
}
