#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include "windows.h"


#define HEIGHT 22
#define LENGHT 13

/*
Вводим систему координат. Ось Х направлена вправо из нулевого элемента (левый верхний угол массива),
ось Y направлена вниз из нулевого элемента. Значит левый верхний угол имеет координаты 0;0. Перемещение по оси Х вправо на 1 осуществляется так:
Field[k+1], а вниз на 1: Field[k+1*LENGHT]
*/

char Field[LENGHT * HEIGHT] = {
		'-','-','-','-','-','-','-','-','-','-','-','-','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','\n',
		'-','-','-','-','-','-','-','-','-','-','-','-','\0'
};

class Figure1 {
protected:
	int i_coords[4]; // Здесь хранятся координаты точек фигуры по оси Х (относительно центра)
	int j_coords[4]; // То же самое, но по оси Y. То есть, координаты точки имеют вид (i_coords[k];j_coords[k]). Это координаты относительно центра!
	int center[2];   // Координаты центра (по сути оси вращения). Координаты центра имеют вид (сenter[0];center[1])
public:
	char ch;
	Figure1(int i1, int i2, int i3, int i4, int j1, int j2, int j3, int j4) : center{ rand() % 5 + 2,2 }, i_coords{ i1,i2,i3,i4 }, j_coords{ j1,j2,j3,j4 }, ch{ 'o' }
	{
	}
	~Figure1() {}
	// Метод для отрисовки фигуры
	virtual void Draw() {
		for (int k = 0; k < 4; k++) {
			Field[i_coords[k] + center[0] + (j_coords[k] + center[1]) * LENGHT] = ch;
		}
		printf("%s", Field);
	}
	// Метод для стирания фигуры
	virtual void Clear() {
		for (int k = 0; k < 4; k++) {
			Field[i_coords[k] + center[0] + (j_coords[k] + center[1]) * LENGHT] = ' ';
		}
	}
	// Метод поворота фигуры. Именно для него я использую систему с координатами относительно центра.
	// Координаты относительно центра по сути - вектора, поэтому, чтобы повернуть фигуру, нужно повернуть все вектора на 90 в одну сторону.
	// Для этого нужно поменять местами компоненты вектора и у одной из них сменить знак
	virtual void Rotate() {
		int buf,z=0;
		if (CheckForBorder(this)==0) {
			Clear();
			for (int k = 1; k < 4; k++) {  // Координаты первой точки всегда 0;0, так как k начинается с 1 (то есть i_coords[0];j_coords[0] это 0;0)
				buf = i_coords[k];
				i_coords[k] = (-1) * j_coords[k];
				j_coords[k] = buf;
			}
			z = 0;
			for (int k = 0; k < 4; k++) {
				if (i_coords[k] + center[0]<2 || i_coords[k] + center[0]>LENGHT - 3) {
					for (int kk = 1; kk < 4; kk++) {
						buf = j_coords[kk];
						j_coords[kk] = (-1)*i_coords[kk];
						i_coords[kk] = buf;
					}
					z = 1;
				}
				if (z) break;
			}
		}
	}
	// Метод для движения фигуры (заодно с отрисовкой)
	virtual void Move(int lr, int *ff) {
		Clear();
		if ((CheckForBorders(this) == -1 || CheckForBorders(this) == 0) && lr > 0) {
			center[0] += lr;
		}
		else if ((CheckForBorders(this) == 1 || CheckForBorders(this) == 0) && lr < 0) {
			center[0] += lr;
		}
		if (*ff == 3) {  // ff - счетчик для падения
			center[1] += 1;
			*ff = 0;
		}
		Draw();
	}
	friend int CheckForBorder(Figure1* point);  // Функция для проверки на достижение нижней границы
	friend int CheckForBorders(Figure1* point); // Функция для проверки на достижение боковых границ
							// не только границ поля, но и границ, образованных уже упавшими фигурами (касается обоих функций)
};


class Figure2 : public Figure1 {  // об этом классе позже
public:
	Figure2() : Figure1(0, -1, 1, 2, 0, 0, 0, 0)  // oooo
	{
	}
	~Figure2() {}
	friend int CheckForBorder(Figure1* point);
	friend int CheckForBorders(Figure1* point);
};





int  CheckForBorder(Figure1* point) {
	// Проверяем: если под точкой фигуры находится не пробел и этот "не пробел", не принадлежит ни одной другой точке фигуры (все это проверяю для каждой точки фигуры)
	int i = 0;
	for (int k = 0; k < 4; k++) {
		if (Field[point->i_coords[k] + point->center[0] + (point->j_coords[k] + point->center[1] + 1) * LENGHT] != ' ') {
			i = 0;
			for (int kk = 0; kk < 4; kk++) {
				if (point->i_coords[k] + (point->j_coords[k] + point->center[1] + 1) * LENGHT !=
					point->i_coords[kk] + (point->j_coords[kk] + point->center[1]) * LENGHT) {
					i++;  // Если "не пробел" под точкой не принадлежит фигуре, то i++
				}
			}
			if (i == 4) return 1; // Если все 4 точки фигуры НЕ принадлежат "не пробелу", то достигнута нижняя границы
		}
	}
	return 0;
}

// Подобным образом идет проверка на боковые границы
// Я сперва написал эту и предыдущую функцию, но потом заметил, что она неправильная и исправил,
// а эту не проверял на корректность, хотя все тесты она показывала себя нормально
int CheckForBorders(Figure1* point) {
	int i = 0;
	for (int k = 0; k < 4; k++) {
		if (Field[point->i_coords[k] + point->center[0] + 1 + (point->j_coords[k] + point->center[1]) * LENGHT] != ' ') {
			i = 0;
			for (int kk = 0; kk < 4; kk++) {
				if (point->i_coords[k] + 1 != point->i_coords[kk]) {
					i++;
				}
			}
			if (i == 4) return 1;
		}
		else if (Field[point->i_coords[k] + point->center[0] - 1 + (point->j_coords[k] + point->center[1]) * LENGHT] != ' ') {
			i = 0;
			for (int kk = 0; kk < 4; kk++) {
				if (point->i_coords[k] - 1 != point->i_coords[kk]) {
					i++;
				}
			}
			if (i == 4) return -1;
		}
	}
	return 0;
}

// Проверка на собранную линию, ее удаление и смещение всего, что сверху вниз
// Все границы в циклах подобраны так, чтобы пробегать только рабочую часть массива (там, шде могут двигаться фигуры, то есть без границ массива)
void CheckForLine() {
	int s;
	for (int k = HEIGHT - 2; k > 0; k--) {
		s = 0;
		for (int kk = 1; kk < LENGHT - 2; kk++) {
			if (Field[kk + k * LENGHT] != ' ') s++;
		}
		if (s == LENGHT - 3) {
			for (int kk = 1; kk < LENGHT - 2; kk++) {
				Field[kk + k * LENGHT] = ' ';
			}
			for (int r = k; r > 1; r--) {
				for (int kk = 1; kk < LENGHT - 2; kk++) {
					Field[kk + r * LENGHT] = Field[kk + (r - 1) * LENGHT];
				}
			}
			k = k + 1;
		}
	}
}

// Функция для выбора фигуры
Figure1* RandSelect() {
	int num = rand() % 5 + 1;
	Figure1* point = nullptr;
	switch (num)
	{
	case 1:
		point = new Figure1(0, 0, -1, 1, 0, -1, 0, 0); // класс Figure1 сделан так, что создавать новую фигуру можно через
							  // конструктор, но нам по заданию нужно иметь в проекте полиморфизм, наследование и 
							  // инкапсуляцию. Инкапсуляция по сути есть, а вот для остального пришлось создавать
							  // костыль с наследованием. Как доказательство создан класс Figure2 
		return point;
		break;
	case 2:
		point = new Figure2();
		return point;
		break;
	case 3:
		point = new Figure1(0, -1, 0, 1, 0, 0, -1, -1);
		return point;
		break;
	case 4:
		point = new Figure1(0, 0, 1, 2, 0, -1, 0, 0);
		return point;
		break;
	case 5:
		point = new Figure1(0, 0, 1, 1, 0, 1, 0, 1);
		return point;
		break;
	default:
		break;
	}

}

int main() {
	srand(time(NULL));
	Figure1* point = RandSelect();
	int ff = 0;
	int lr;
	do {
		lr = 0;
		if (GetAsyncKeyState(VK_LEFT)) lr = -1;   // Функция для проверки нажатия клавиши
		else if (GetAsyncKeyState(VK_RIGHT)) lr = 1; // в lr записываем инфу о движении фигуры по оси Х
		else if (GetAsyncKeyState(VK_UP)) point->Rotate();
		else if (GetAsyncKeyState(VK_DOWN)) ff = 3; // Раз в 3 тика фигура падает на одну клетку, но при нажатии "вниз" она падает досрочно
		point->Move(lr, &ff);
		if (CheckForBorder(point) == 1) {  // если достигнут низ, то...
			delete point;
			CheckForLine();
			point = RandSelect();
		}
		Sleep(50);
		ff++;
		system("cls");  // очистка консоли
	} while (1);
	return 0;
}
