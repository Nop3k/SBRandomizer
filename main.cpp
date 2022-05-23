#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>

void DisplayMenu();
void Menu(int choice, std::map<std::string, int> &lottery_players, std::map<std::string, std::vector<int>> &lottery_players_tickets);
std::vector<std::string> TokenizeCSVLine(const std::string line);
void GenerateWinners(std::map<std::string, std::vector<int>> &lottery_players_tickets);
std::string FindWinner(const std::map<std::string, std::vector<int>> &lottery_players_tickets, const int winning_number);
std::map<std::string, int> GenerateLotteryPlayers(const std::string file_name);
std::vector<int> GenerateTicketPool(const int ticket_count);
std::map<std::string, int> EditLotteryPlayers(std::map<std::string, int> &lottery_players);
void PrintLotteryPlayers(const std::map<std::string, int> &lottery_players);
void PrintLotteryPlayersTickets(const std::map<std::string, std::vector<int>> &lottery_players_tickets);
std::map<std::string, std::vector<int>> GenerateLotteryPlayersTicketsEven(const std::string file_name, const std::map<std::string, int> &lottery_players);
std::map<std::string, std::vector<int>> GenerateLotteryPlayersTicketsBattles(const std::string file_name, const std::map<std::string, int> &lottery_players);

int main()
{
    std::map<std::string, int> lottery_players;
    std::map<std::string, std::vector<int>> lottery_players_tickets;
    int choice;

    do
    {
        DisplayMenu();
        std::cin >> choice;
        Menu(choice, lottery_players, lottery_players_tickets);
    } while (choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5 || choice == 6);
    std::cout << std::endl;
}

std::vector<std::string> TokenizeCSVLine(const std::string line)
{
    std::vector<std::string> tokens;
    int start = 0;
    int end = line.find(';');
    while (end != std::string::npos)
    {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(';', start + 1);
    }
    tokens.push_back(line.substr(start, end - start));

    return tokens;
}

void PrintLotteryPlayers(const std::map<std::string, int> &lottery_players)
{
    int i = 1;

    system("cls");

    for (auto member : lottery_players)
        std::cout << i++ << ". " << member.first << " : " << member.second << std::endl;
}

void PrintLotteryPlayersTickets(const std::map<std::string, std::vector<int>> &lottery_players_tickets)
{
    int i = 1;

    system("cls");

    for (auto member : lottery_players_tickets)
    {
        std::cout << i++ << ". " << member.first << " : ";
        for (auto number : member.second)
            std::cout << number << ' ';
        std::cout << std::endl;
    }
}

void GenerateWinners(std::map<std::string, std::vector<int>> &lottery_players_tickets)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(1, lottery_players_tickets.size());

    std::vector<std::string> winners;
    std::string possible_winner;
    int generated_number, number_of_prizes;

    system("cls");

    std::cout << "How many prizes?: ";
    if ((std::cin >> number_of_prizes) && (number_of_prizes <= lottery_players_tickets.size()))
    {

        do
        {
            generated_number = distr(generator);
            possible_winner = FindWinner(lottery_players_tickets, generated_number);
            if (std::find(winners.begin(), winners.end(), possible_winner) == winners.end())
            {
                winners.push_back(possible_winner);
                std::cout << "Chosen number is: " << generated_number << " And the winner is... " << winners.back() << std::endl;
            }
        } while (winners.size() < number_of_prizes);
    }
    else
        std::cout << "Bad input, try again.\n";
    return;
}

std::string FindWinner(const std::map<std::string, std::vector<int>> &lottery_players_tickets, const int winning_number)
{
    for (auto member : lottery_players_tickets)
        for (auto number : member.second)
            if (number == winning_number)
                return member.first;
    return "";
}

std::map<std::string, int> GenerateLotteryPlayers(const std::string file_name)
{
    std::map<std::string, int> lottery_players;
    std::string line;

    system("cls");

    std::fstream file(file_name, std::ios::in);
    if (file.is_open())
    {
        std::getline(file, line, '\n');
        while (std::getline(file, line, '\n'))
            lottery_players.insert({TokenizeCSVLine(line)[0], 1});
        file.close();
    }
    else
        std::cout << "Could not open the file\n";

    std::cout << "Created " << lottery_players.size() << " player list.\n";
    return lottery_players;
}

std::vector<int> GenerateTicketPool(int ticket_count)
{
    std::vector<int> ticket_pool(ticket_count);
    std::cout << "Generating " << ticket_count << " tickets pool.\n";
    std::iota(ticket_pool.begin(), ticket_pool.end(), 1);
    std::shuffle(ticket_pool.begin(), ticket_pool.end(), std::random_device());
    return ticket_pool;
}

std::map<std::string, int> EditLotteryPlayers(std::map<std::string, int> &lottery_players)
{
    int number;
    PrintLotteryPlayers(lottery_players);

    std::cout << "Which player to remove? (q to quit) : ";
    std::cin >> number;
    if ((number >= 1) && (number <= lottery_players.size()))
    {
        auto player_iterator = lottery_players.begin();
        std::advance(player_iterator, number - 1);
        lottery_players.erase(player_iterator);
    }

    PrintLotteryPlayers(lottery_players);
    return lottery_players;
}

std::map<std::string, std::vector<int>> GenerateLotteryPlayersTicketsEven(const std::string file_name, const std::map<std::string, int> &lottery_players)
{
    std::map<std::string, std::vector<int>> lottery_players_tickets;
    std::vector<int> ticket_pool = GenerateTicketPool((int)lottery_players.size());

    system("cls");

    int i = 0;
    for (auto player : lottery_players)
    {
        lottery_players_tickets.insert({player.first, {ticket_pool[i++]}});
    }

    std::cout << "Succesfully generated " << lottery_players.size() << " tickets for " << lottery_players.size() << " players.\n";

    return lottery_players_tickets;
}

std::map<std::string, std::vector<int>> GenerateLotteryPlayersTicketsBattles(const std::string file_name, const std::map<std::string, int> &lottery_players)
{
    std::map<std::string, std::vector<int>> lottery_players_tickets;
    std::map<std::string, int> players_battletickets;
    std::string line, player;
    int activity, battles_per_ticket, total_tickets = 0;

    system("cls");

    std::cout << "How many battles per ticket?: ";
    if (std::cin >> battles_per_ticket)
    {

        std::fstream file(file_name, std::ios::in);
        if (file.is_open())
        {
            std::getline(file, line, '\n');
            while (std::getline(file, line, '\n'))
            {
                player = TokenizeCSVLine(line)[0];
                activity = std::stoi(TokenizeCSVLine(line)[3]);
                if ((activity >= battles_per_ticket) && (lottery_players.find(player) != lottery_players.end()))
                {
                    total_tickets += activity / battles_per_ticket;
                    players_battletickets.insert({player, activity / battles_per_ticket});
                }
            }
            file.close();
            std::vector<int> ticket_pool = GenerateTicketPool(total_tickets);
            std::vector<int> player_ticket_pool;
            int used_count = 0;
            for (auto row : players_battletickets)
            {
                player_ticket_pool.clear();
                for (int i = 0; i < row.second; ++i)
                    player_ticket_pool.push_back(ticket_pool[used_count++]);
                lottery_players_tickets.insert({row.first, player_ticket_pool});
            }
            std::cout << "Succesfully generated " << total_tickets << " tickets for " << players_battletickets.size() << " players.\n";
        }
        else
            std::cout << "Could not open the file\n";
    }
    else
    {
        std::cout << "Input problem, try again.\n";
        std::cin.clear();
        std::cin.ignore();
    }
    return lottery_players_tickets;
}

void DisplayMenu()
{
    std::cout << "======Menu======\n";
    std::cout << "1: Generate Player List\n";
    std::cout << "2: Edit Player List\n";
    std::cout << "3: Generate Players and Tickets - even chances\n";
    std::cout << "4: Generate Players and Tickets - tickets per battles\n";
    std::cout << "5: Show me Players and Tickets\n";
    std::cout << "6: Get winner!\n";
    std::cout << "Anything else to quit.\n";
    return;
}
void Menu(int choice, std::map<std::string, int> &lottery_players, std::map<std::string, std::vector<int>> &lottery_players_tickets)
{
    std::string file_name = "ggfights-pl6.csv";
    switch (choice)
    {
    case 1:
        lottery_players = GenerateLotteryPlayers(file_name);
        break;
    case 2:
        lottery_players = EditLotteryPlayers(lottery_players);
        break;
    case 3:
        lottery_players_tickets = GenerateLotteryPlayersTicketsEven(file_name, lottery_players);
        break;
    case 4:
        lottery_players_tickets = GenerateLotteryPlayersTicketsBattles(file_name, lottery_players);
        break;
    case 5:
        PrintLotteryPlayersTickets(lottery_players_tickets);
        break;
    case 6:
        GenerateWinners(lottery_players_tickets);
        break;
    default:
        std::cout << "Quitting bye\n";
        break;
    }
    return;
}