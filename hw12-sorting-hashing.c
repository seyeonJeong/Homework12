/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a);
int freeArray(int *a);
void printArray(int *a);

int selectionSort(int *a);
int insertionSort(int *a);
int bubbleSort(int *a);
int shellSort(int *a);
/* recursive function으로 구현 */
int quickSort(int *a, int n);


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key);

/* array a에대 한 hash table을 만든다. */
int hashing(int *a, int **ht);

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key);


int main()
{
	char command;
	int *array = NULL;
	int *hashtable = NULL;
	int key = -1;
	int index = -1;

	srand(time(NULL));

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&array);
			break;
		case 'q': case 'Q':
			freeArray(array);
			break;
		case 's': case 'S':
			selectionSort(array);
			break;
		case 'i': case 'I':
			insertionSort(array);
			break;
		case 'b': case 'B':
			bubbleSort(array);
			break;
		case 'l': case 'L':
			shellSort(array);
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			quickSort(array, MAX_ARRAY_SIZE);
			printf("----------------------------------------------------------------\n");
			printArray(array);

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array);
			hashing(array, &hashtable);
			printArray(hashtable);
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key);
			printArray(hashtable);
			index = search(hashtable, key);
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) {
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*a = temp;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else
		temp = *a;

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		temp[i] = rand() % MAX_ARRAY_SIZE;

	return 0;
}

int freeArray(int *a)
{
	if (a != NULL) //만약 a가 NULL이 아닐경우
		free(a); //a를 해제해준다
	return 0;
}

void printArray(int *a)
{
	if (a == NULL) { //a의 값이 없다면 경고문구를 출력하고 return
		printf("nothing to print.\n");
		return;
	}
	// a[i]의 값을 출력해줌
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i);
	printf("\n");
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a)
{
	//선택정렬
	int min; //최소값
	int minindex; //최소값이 존재하는 인덱스
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (i = 0; i < MAX_ARRAY_SIZE; i++) //모든인덱스에대하여 반복해준다
	{
		minindex = i; //minindex값을 i로 설정
		min = a[i]; //min값을 a[i]의 값으로 설정
		for (j = i + 1; j < MAX_ARRAY_SIZE; j++)
		{ //인덱스 i 다음 인덱스부터 탐색
			if (min > a[j])  //만약 min값보다 a[j]의 값이 더 작다면
			{
				min = a[j]; //min값은 a[j]로 설정
				minindex = j; //최소인덱스는 j
			}
		}
		a[minindex] = a[i]; //탐색을 마치고 가장 작은 값이 있던 자리에 a[i]값을 대입
		a[i] = min; //a[i]의 값은 min값이됨  ( ex) 가장 작으면 a[0] = min)
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}

int insertionSort(int *a)
{
	//삽입 정렬 : 자료 배열의 모든 요소를 앞에서부터 차례대로 이미 정렬된 배열 부분과 비교하여 자리를 찾아 삽입함으로써 정렬을 완성
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i]; //우선 t를 a[i]의 값으로 설정한다.
		j = i; //j를 i의 값으로 설정한다.
		while (a[j - 1] > t && j > 0) //배열에서 앞에 있는 배열이 더 크고, j가 0보다 크다면 반복(앞과 뒤의 배열을 계속해서 비교해준다)
		{
			a[j] = a[j - 1]; //뒤에있는것을 앞의 배열의 자리로 옮겨줌
			j--; //j = j-1의 연산을 수행
		}
		a[j] = t; // a[j]의 값에 t를 대입
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j])
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h];
					k -= h;
				}
				a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);
			while(a[--j] > v);

			if (i >= j) break;
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
	}


	return 0;
}

int hashCode(int key) {
	return key % MAX_HASH_TABLE_SIZE; //key 값에 MAX_HASH_TABLE_SIZE를 모듈연산하여 hashCode를 생성함
}

int hashing(int *a, int **ht)
{
	int* hashtable = NULL;

	/* hash table이 NULL인 경우 메모리 할당 */
	if (*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	}
	else {
		hashtable = *ht;	/* hash table이 NULL이 아닌경우, table 재활용, reset to -1 */
	}

	for (int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	/*
	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		printf("hashtable[%d] = %d\n", i, hashtable[i]);
	*/
	//변수를 초기화 해준다.
	int key = -1;
	int hashcode = -1;
	int index = -1;
	//랜덤으로 생성한 값들에 hashcode를 할당해서 hashtable에 삽입해준다.
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i]; //key는 a[i]에 들어있는 정수값
		hashcode = hashCode(key); //hashcode를 만들어주는 함수에 key값을 삽입한다.
		/*
		printf("key = %d, hashcode = %d, hashtable[%d]=%d\n", key, hashcode, hashcode, hashtable[hashcode]);
		*/
		if (hashtable[hashcode] == -1) //만약 hashtable[hashcode]에 해당하는 값이 -1이라면(hashtable이 비어있다면)
		{
			hashtable[hashcode] = key; //hashtable[hashcode]에 해당하는 장소에 key값을 삽입
		}
		else {
			 //hashcode의 인덱스값에 해당하는 hashtable이 비어있지 않다면
			index = hashcode; // index는 hashcode로 설정

			while (hashtable[index] != -1) //선형 조사법을 사용하여 빈 인덱스값을 찾는다.
			{
				index = (++index) % MAX_HASH_TABLE_SIZE; //index는 index+1 을 해준 값에 MAX_HASH_TABLE_SIZE를 모듈연산 해준값으로 설정해준다.
				/*
				printf("index = %d\n", index);
				*/
			}
			hashtable[index] = key; // 빈 인덱스 값을 찾았다면 그 인덱스값에 해당하는 hashtable에 key값을 삽입
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key); //index의 값을 key값을 hashCode함수에 넣은 값으로 변경

	if (ht[index] == key) //만약 ht[index]값이 key값이라면(찾는 값이라면)
		return index; //index값을 return한다

	while (ht[++index] != key) //index값이 key값이 아니라면(이런 경우면 같은 hashcode에 해당하는 값이 정해진 index값에 들어간 경우가 아니라 선형조사법을 이용해서 삽입해준 경우이므로 선형조사법처럼 모듈연산을 이용하여 ht[index]값이 key값이 될때까지 탐색한다.
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index; //index값을 return 한다.
}



