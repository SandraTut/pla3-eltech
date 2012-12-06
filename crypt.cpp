#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std; 

int main(int argc, char const *argv[])
{

	int i; // номер параметра или значения параметра в массиве
	FILE *source; // входной файл
	FILE *dest; // выходной файл
	string alphabet = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789"; // алфавит
	int key; // Значение ключа
	int keyNumb; // Номер ключа с конца в массиве введенных параметров
	bool ColNumb = false; // Файлы входной и выходной совпадают - true
	bool code = false; // шифровать(false) или расшифровать(true) 
	char symbol;
	int index; // номер символа в входном тексте
	int indexNew; // номер символа в выходном файле
	string argv_i; // вспомогательная переменная
	string argv_do_i; // вспомогательная переменная
	string argv_posle_i; // вспомогательная переменная
	string stdSource; // хранит имя входного файла
	string stdDest; // хренит имя выходного файла
	

	// если кол-во параметров 2, один из которых --help/-h, то программа выводит справку
	if (argc == 2)
	{
		argv_i = argv[1];
		if ((argv_i == "-h") || (argv_i == "--help"))
		{
			printf ("%s\n","$ crypt [options] <key> <source> [<dest>]");
            printf ("%s\n","options:");
            printf ("%s\n"," -a, --alphabet=<alphabet>");
            printf ("%s\n","alphabet — алфавит для работы алгоритма (по умолчанию содержит буквы из латниского алфавита и цифры: AaBbCc..Zz0..9)");
            printf ("%s\n","-t, --type=<type>"); 
            printf ("%s\n","type может быть 'encode' или 'decode', по умолчанию — encode");
            printf ("%s\n","-h, --help");
            printf ("%s\n","выводит эту справку");
            printf ("%s\n","key: ключ для шифрования/дешифрования");
            printf ("%s\n","source: исходный файл для шифрования/дешифрования");
            printf ("%s\n","dest: файл, куда будет записан новый, зашифрованный текст. Если не указан, то переписывает source");	
            return 0;
		}
	}
	if (argc > 2 && argc <= 8) // если кол-во параметров от 2 и до 8, то начинаем чтение параметров.
	{	
		// Проверяем правильность ввода параметров. Сначала только обязательную часть, чтобы после нахождения их 
		// было возможно их использовать как константы в цикле 

		// ----------------ПРОВЕРКА ОБЯЗАТЕЛЬНОЙ ЧАСТИ ПРОГРАММЫ (source, dest(?), keys)----------
		// если значение последнего параметра открывается на чтение, то записать его в source, 
		// если нет, то выводим сообщение об ошибке
		if (dest = fopen(argv[argc-1], "r")) 
		{
			// Если значение предпоследнего параметра открывается на чтение, то перезаписать файл source и записать dest
			if (source = fopen(argv[argc-2], "r"))
			{
				fclose (dest); 
				dest = fopen(argv[argc-1],"w");
				keyNumb = argc-3;
			}
			// Иначе записать значение последнего параметра в source
			else
			{
				fclose (dest);
				dest = fopen(argv[argc-1],"r+");
				keyNumb = argc-2;
				ColNumb = true;
			}
		}
		// Сообщение об ошибке, если не открылись файлы входные и/или выходные
		else
		{
			printf("Неверно введены параметры\n");
			return -1; 
		}
		
		// Если ключ целочисленный, то записываем значение ключа. Иначе выводим сообщение об ошибке
		if (!(key = atoi(argv[keyNumb])) || key < 1) 
		{
			printf("Неверно введены параметры\n"); 
			return -1;
		}
	}
	// Сообщение об ошибке, если кол-во параметров больше/меньше нужных
	else 
	{ 
		printf("Неверно введены параметры\n");
		return -1;
	}
	// -------------ПРОВЕРКА НЕОБЯЗАТЕЛЬНОЙ ЧАСТИ ПРОГРАММЫ (-a, --alphabet=, -t, --type=, -h, --help)---------
	for (i = 1; i < argc; i++) 
	{	
		// Если указан параметр -a, номер которого в массиве не совпдает с номером ключа
		string argv_i = argv[i];
		string argv_do_i = argv[i-1];
		string argv_posle_i = argv[i+1];		
		if (argv_i == "-a")
		{
			// Проверим, не является ли след. после -a параметр ключом. Если нет, то записываем следующий параметр, 
			// как алфавит кодировки. Если да, то пользуемся алфавитом по умолчанию
			if (i+1 != keyNumb)
			{ 
				i++;
				alphabet = argv[i];
				continue;
			}
			else 
			{ 
				printf("Неверно введены параметры\n");
				return -1;
			}
		}
		// Если указанный параметр содержит --alphabet= и его номер в массиве не сопрадает с номером ключа, то
		// берем все, что после "=" и записываем в алфавит.
		if (argv_i.find("--alphabet=") + 1)
		{
			// Записываем в alphabet все символы после "="
			alphabet = argv_i.erase(0,11);
		}
		// Если указанный параметр -t, то смотрим следующее после него 
		// значение парметра
		if (argv_i == "-t")
		{
			// Проверяем следующий параметр. Если его номер не совпадает с номером ключа, то смотрим его значение.
			// Если совпадает, то ошибка.
			if (i+1 != keyNumb && (argv_posle_i == "encode" || argv_posle_i == "decode"))
			{
				// если encode - расшифровка, если decode - зашифровка
				if (argv_posle_i == "encode"){ code = 1; }	
				if (argv_posle_i == "decode"){ code = 2; }			
			}
			// Неверно введены параметры
			else 
			{ 
				printf("Неверно введены параметры\n");
				return -1;
			}
				}
			// Если указанный параметр содержит --type=, то смотрим то, что после "="
			if (argv_i.find("--type=") + 1)
				{
					// если после равно есть encode иди decode присваиваем нужное число переменной code
					if (argv_i.find("encode") + 1)
					{
						code = 1; 
					}
					if (argv_i.find("decode") + 1)
					{
						code = 2; 
					}
				}		
		}
	//-------------------------------РЕАЛИЗАЦИЯ ШИФРА-------------------------------------
	if(code) key *= -1;
	while((symbol=fgetc(source)) != EOF)
	{
		if(!(ColNumb)) fseek ( dest , -1 , SEEK_CUR );
		if((index = alphabet.find(symbol)) +1)
		{
			if (key > alphabet.length())
			{
				key = key%alphabet.length();
			}
			indexNew = (index + key)%alphabet.length();
			symbol=alphabet[indexNew];
		}
		fputc(symbol,dest);
	}
	fclose(source);
	if(ColNumb) fclose(dest);
	return 0;
}
	

