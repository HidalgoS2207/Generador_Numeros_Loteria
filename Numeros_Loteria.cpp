#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <map>

//forward declarations --------------------------------------------
void load_numbers_from_file(std::vector<int*> * num_container); 
void get_next_numbers(std::fstream * fst, std::vector<int> * numbers);
//----------------------------------------------------------------------

void generate_lottery_numbers(int output_number)
{
	std::default_random_engine generator((int)time(NULL));
	std::uniform_int_distribution<int> aleatory_number(0, 45);
	//std::uniform_int_distribution<int> from_list(0, SERIES - 1);

	std::vector<int*> out_num;

	std::fstream fst;

	std::string aux_string;

	//int ** blocks_container = new int*;
	int * base;
	int aux, index;

	fst.open("Winning_Numbers.txt", std::ios::out);

	if (!fst.good())
	{
		std::cout << "Error. File couldn't be opened!" << std::endl;
	}
	else
	{
		for (int i = 0; i < output_number; i++)
		{
			base = new int[6];

			for (int j = 0; j < 6; j++)
			{
				index = 0;
				aux = aleatory_number(generator);

				while (index <= j)
				{
					if (aux == base[index])
					{
						aux = aleatory_number(generator);

						index = -1;
					}

					index++;
				}

				base[j] = aux;
			}

			//blocks_container[i] = base;

			std::sort(base, base + 6);

			fst.write("{", 1);

			for (int j = 0; j < 6; j++)
			{
				aux_string = std::to_string(base[j]);

				if (base[j] > 9)
				{
					fst.write(aux_string.c_str(), 2);
				}
				else
				{
					fst.write(aux_string.c_str(), 1);
				}


				if (j != 5)
				{
					fst.write(" , ", 3);
				}
			}

			fst.write("}", 1);
			fst.write("\n\n", 2);

			delete[] base;
		}
	}

	fst.close();

}
void compare_winning_numbers()
{
	std::vector<int> numbers_containers;
	std::vector<int*> coincidences;
	std::vector<int> winning_numbers;
	int win_num = 0;

	std::pair<int,int> aux;
	std::vector< std::pair<int, int> > index_coincidences;

	int numbers_to_show = 0;
	int action = 0;
	bool correct_index = false;

	std::map<int, int> oc_num;

	int coincidences_counter = 0;

	std::fstream fst;
	fst.open("Winning_Numbers.txt");

	if (!fst.good())
	{
		std::cout << "\nError. Couldn't open file." << std::endl;
	}
	else
	{
		get_next_numbers(&fst, &numbers_containers);

		if (numbers_containers.size() == 0)
		{
			std::cout << "\nNo numbers found in registry." << std::endl;

			fst.close();
		}
		else
		{
			std::cout << "\nSet the winning numbers:\n " << std::endl;

			for (int i = 0; i < 6; i++)
			{
				std::cout << i + 1 << " - ";
				std::cin >> win_num;

				while (win_num < 0 || win_num > 45)
				{
					std::cout << "Number out of limits. Insert number again:" << std::endl;
					std::cout << i + 1 << " - ";
					std::cin >> win_num;
				}

				winning_numbers.push_back(win_num);
				win_num = 0;
			}

			std::sort(winning_numbers.begin(), winning_numbers.end());

			while (!fst.eof())
			{
				coincidences_counter = 0;

				for (int i = 0; i < 6; i++)
				{

					for (int k = 0; k < 6; k++)
					{
						if (numbers_containers[i] == winning_numbers[k])
						{
							coincidences_counter++;
						}
						else if (numbers_containers[i] < winning_numbers[k])
						{
							break;
						}
					}
				}

				if (coincidences_counter != 0)
				{
					if (coincidences_counter > 3)
					{
						int * aux_container = new int[6];

						for (int j = 0; j < 6; j++)
						{
							aux_container[j] = numbers_containers[j];
						}

						coincidences.push_back(aux_container);

						aux.first = coincidences.size() - 1;
						aux.second = coincidences_counter;

						index_coincidences.push_back(aux);

						//delete[] aux_container; NO HACER ESTO PORQUE SE PIERDE LA INFORMACION EN coincidences[]
					}

					oc_num[coincidences_counter]++;
				}

				numbers_containers.clear();
				get_next_numbers(&fst, &numbers_containers);
			}

			fst.close();

			std::cout << "\n\nRESULTS:" << std::endl;
			if (oc_num.empty())
			{
				std::cout << "\nThere is no coincidences in any played numbers.\n";
			}
			else
			{
				for (auto it = oc_num.begin(); it != oc_num.end(); ++it)
				{
					std::cout << "\n" << it->second << " tickets with " << it->first << " coincidences.";
				}

				std::cout << "\n\nChoose an action: \n1 - List specific tickets by number of coincidences.\n2 - List all tickets with coincidences\n3 - Exit\n\n --> ";
				std::cin >> action;
				while (action > 3 || action < 1)
				{
					std::cout << "\nOut of range. Choose an action:\n\n";
					std::cin >> action;
				}

				switch (action)
				{
				case 1:
					std::cout << "\nSelect the series to list by number of coincidences: \n";
					std::cin >> numbers_to_show;
					while (!correct_index)
					{
						if (numbers_to_show > 6 || numbers_to_show < 4)
						{
							std::cout << "Out of range. Choose again.\n\n";
							correct_index = false;
							std::cin >> numbers_to_show;
						}
						else
						{
							correct_index = false;
							for (auto it = oc_num.begin(); it != oc_num.end(); ++it)
							{
								if (it->first == numbers_to_show)
								{
									correct_index = true;
								}
							}

							if (!correct_index)
							{
								std::cout << "\nThere's no tickets with that number of coincidences in the registry. Try another number\n";
								std::cin >> numbers_to_show;
								break;
							}
						}
					}

					std::cout << std::endl << std::endl;
					if (correct_index)
					{
						for (int i = 0; i < coincidences.size(); i++)
						{
							if (index_coincidences[i].second == numbers_to_show)
							{
								std::cout << index_coincidences[i].second << " Coincidences -> \n\nWinning numbers: ";
								for (int j = 0; j < 6; j++)
								{
									std::cout << winning_numbers[j] << " ";
								}
								std::cout << "\n\nPlayed Numbers: ";
								for (int j = 0; j < 6; j++)
								{
									std::cout << coincidences[index_coincidences[i].first][j] << " ";
								}
								std::cout << std::endl << std::endl;
							}
						}
					}

					break;
				case 2:
					std::cout << std::endl << std::endl;
					for (int i = 0; i < coincidences.size(); i++)
					{
						std::cout << index_coincidences[i].second << " Coincidences -> \n\nWinning numbers: ";
						for (int j = 0; j < 6; j++)
						{
							std::cout << winning_numbers[j] << " ";
						}
						std::cout << "\n\nPlayed Numbers: ";
						for (int j = 0; j < 6; j++)
						{
							std::cout << coincidences[index_coincidences[i].first][j] << " ";
						}
						std::cout << std::endl << std::endl;
					}
					break;
				case 3:
					break;
				}
			}
		}
	}

	for (int i = 0; i < coincidences.size(); i++)
	{
		delete[] coincidences[i];
	}
}
void get_current_list()
{
	std::vector<int*> numbers_containers;

	load_numbers_from_file(&numbers_containers);

	std::cout << std::endl;

	if (numbers_containers.size() != 0)
	{
		for (int i = 0; i < numbers_containers.size(); i++)
		{
				std::cout << numbers_containers[i][0] << " " << numbers_containers[i][1] << " " << numbers_containers[i][2] << " " << numbers_containers[i][3] << " " << numbers_containers[i][4] << " " << numbers_containers[i][5] << " " <<"\n\n";
		}
	}
}
void compare_manually()
{
	int win_num = 0;
	std::vector<int> winning_numbers;
	std::vector<int> ticket_numbers;

	int coincidences_counter = 0;

	std::cout << "Set the played ticket numbers: " << std::endl;

	for (int i = 0; i < 6; i++)
	{
		std::cout << i + 1 << " - ";
		std::cin >> win_num;

		while (win_num < 0 || win_num > 45)
		{
			std::cout << "Number out of limits. Insert number again:" << std::endl;
			std::cout << i + 1 << " - ";
			std::cin >> win_num;
		}

		ticket_numbers.push_back(win_num);
		win_num = 0;
	}

	std::sort(ticket_numbers.begin(), ticket_numbers.end());

	std::cout << "Set the winning numbers: " << std::endl;

	for (int i = 0; i < 6; i++)
	{
		std::cout << i + 1 << " - ";
		std::cin >> win_num;

		while (win_num < 0 || win_num > 45)
		{
			std::cout << "Number out of limits. Insert number again:" << std::endl;
			std::cout << i + 1 << " - ";
			std::cin >> win_num;
		}

		winning_numbers.push_back(win_num);
		win_num = 0;
	}

	std::sort(winning_numbers.begin(), winning_numbers.end());

	coincidences_counter = 0;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (ticket_numbers[i] < winning_numbers[j])
			{
				break;
			}
			else
			{
				if (ticket_numbers[i] == winning_numbers[j])
				{
					coincidences_counter++;
				}
			}
		}
	}

	std::cout <<"\n\n"<< coincidences_counter << " Coincidences -> \n\nWinning numbers: ";
	for (int j = 0; j < 6; j++)
	{
		std::cout << winning_numbers[j] << " ";
	}
	std::cout << "\n\nTicket Numbers: ";
	for (int j = 0; j < 6; j++)
	{
		std::cout << ticket_numbers[j] << " ";
	}
	std::cout << std::endl << std::endl;
}

int main()
{
	int action = INT_MAX;

	std::cout << "LOTTERY NUMBERS GENERATOR\n\n\n";

	while (action != 0)
	{
		if (action != INT_MAX)
		{
			std::cout << "\n\n-------------------------------------------------------------------------------\n\n";
		}

		std::cout << "Choose an Action:\n " << std::endl;
		std::cout << "0 - EXIT" << std::endl;
		std::cout << "1 - Generate new lottery numbers." << std::endl;
		std::cout << "2 - Compare winning number with last generated numbers." << std::endl;
		std::cout << "3 - List last generated numbers." << std::endl;
		std::cout << "4 - Compare manually a ticket with winning numbers.\n\n--> ";

		std::cin >> action;

		while (action < 0 || action > 4)
		{
			std::cout << "Wrong action number. Choose an Action: \n--> ";
			std::cin >> action;
		}

		switch (action)
		{
		case 0:
			break;
		case 1:
			std::cout << "\nDefine the number of lottery ticket that you want to generate: " << std::endl << "\n--> ";
			std::cin >> action;
			generate_lottery_numbers(action);
			break;
		case 2:
			compare_winning_numbers();
			break;
		case 3:
			get_current_list();
			break;
		case 4:
			compare_manually();
			break;
		}
	}

	return 0;
}

void load_numbers_from_file(std::vector<int*> * num_container)
{
	std::fstream fst;
	std::string read_number;

	fst.open("Winning_Numbers.txt", std::ios::in);

	if (!fst.good())
	{
		std::cout << "\nError. Couldn't open file." << std::endl;
	}
	else
	{
		bool aux;
		std::string aux_str;

		while (!(fst.eof()))
		{
			fst.seekg(1, fst.cur);

			int * base = new int[6];

			for (int i = 0; i < 6; i++)
			{
				read_number.push_back(fst.get());

				if (fst.peek() != ' ')
				{
					read_number.push_back(fst.get());

					if (i != 5)
					{
						fst.seekg(3, fst.cur);
					}
					else
					{
						fst.seekg(5, fst.cur);

						if(fst.peek() != '{')
						{
							aux_str.push_back(fst.get());
							aux = fst.eof();
						}
					}
				}
				else
				{
					if (i != 5)
					{
						fst.seekg(3, fst.cur);
					}
					else
					{
						fst.seekg(5, fst.cur);

						if (fst.peek() != '{')
						{
							aux_str.push_back(fst.get());
							aux = fst.eof();
						}
					}
					
				}

				base[i] = std::stoi(read_number, nullptr, 10);

				read_number.clear();
			}

			num_container->push_back(base);
		}

		fst.close();
	}
}
void get_next_numbers(std::fstream * fst , std::vector<int> * numbers)
{
	std::string read_number;

	if (!(fst->eof()))
	{
		fst->seekg(1, fst->cur);

		for (int i = 0; i < 6; i++)
		{
			read_number.push_back(fst->get());

			if (fst->peek() != ' ' && fst->peek() != '}')
			{
				read_number.push_back(fst->get());

				if (i != 5)
				{
					fst->seekg(3, fst->cur);
				}
				else
				{
					fst->seekg(5, fst->cur);

					if (fst->peek() != '{')
					{
						fst->get();
					}
				}
			}
			else
			{
				if (i != 5)
				{
					fst->seekg(3, fst->cur);
				}
				else
				{
					fst->seekg(5, fst->cur);

					if (fst->peek() != '{')
					{
						fst->get();
					}
				}

			}

			numbers->push_back(std::stoi(read_number, nullptr, 10));

			read_number.clear();
		}
	}
	else
	{
		std::cout << "\nEnd of registry reached." << std::endl;
	}
}