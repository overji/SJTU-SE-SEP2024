#include "lexicon.h"
#include <iostream>
#include <vector>
#include <filesystem>

class mainGame
{
private:
    Lexicon * englishWords;
    Lexicon * foundWords;
    Lexicon * player2Words;
    Lexicon * copyFoundWords;
    std::vector<std::vector<char>>mapOfGame;
    int N;
    int firstPlayerScore;
    int secondPlayerScore;
public:
    mainGame()
    {
        try
        {
            englishWords = new Lexicon("EnglishWords.txt");
        }
        catch (std::runtime_error* er)
        {
            std::filesystem::path curFilePath(__FILE__);
            std::filesystem::path curPath = curFilePath.parent_path();
            std::string myPath = curPath.string() + "/EnglishWords.txt";
            englishWords = new Lexicon(myPath);
        }
        foundWords = new Lexicon();
        player2Words = new Lexicon();
        copyFoundWords = new Lexicon();
        this->firstPlayerScore = 0;
        this->secondPlayerScore = 0;
    }

    void findEnglishWords(int curRow,int curCol,std::string curStr,std::vector<bool>passed)
    {
        curStr += mapOfGame[curRow][curCol];
        passed[curRow * N + curCol] = true;
        if(curStr.size() >= 4 && englishWords->contains(curStr) && !foundWords->contains(curStr))
        {
            foundWords->add(curStr);
            player2Words->add(curStr);
            copyFoundWords->add(curStr);
        }
        else if(!englishWords->containsPrefix(curStr) && !englishWords->contains(curStr))
        {
            return;
        }
        for(int r = std::max(0,curRow - 1);r <= std::min(N-1,curRow + 1);r ++)
        {
            for(int c = std::max(0,curCol - 1);c <= std::min(N-1,curCol + 1);c ++)
            {
                if(passed[r*N + c])continue;
                else
                {
                    findEnglishWords(r,c,curStr,passed);
                }
            }
        }
    }

    static void printWord(const std::string& str)
    {
        std::string upper;
        for(char ch:str)
        {
            upper += (ch >= 'A' && ch <= 'Z')?ch:(ch -'a' + 'A');
        }
        std::cout << upper << " ";
    }

    void initMap()
    {
        std::cin >> N;
        mapOfGame.resize(N);
        for(int i = 0;i < N;i ++)
        {
            mapOfGame[i].resize(N);
        }
        std::cin.ignore();
        for(int r = 0;r < N;r ++)
        {
            for(int c = 0;c < N;c ++)
            {
                std::cin.get(mapOfGame[r][c]);
            }
            std::cin.ignore();
        }
    }

    void initWordList()
    {
        std::vector<bool>passed(N * N);
        for(int r = 0;r < N;r ++)
        {
            for(int c = 0;c < N;c ++)
            {
                std::string curStr;
                findEnglishWords(r,c,curStr,passed);
            }
        }
    }

    void gameProcess(int player)
    {
        while(true)
        {
            std::cout << "Player " << player <<  " Score: " << ((player == 1)?firstPlayerScore:secondPlayerScore)<< std::endl;
            std::string possibleWord;
            std::cin >> possibleWord;
            if(possibleWord == "???")
            {
                break;
            }
            if(possibleWord.size() < 4)
            {
                std::cout << possibleWord << " is too short." << std::endl;
            }
            else if(!englishWords->contains(possibleWord))
            {
                std::cout << possibleWord << " is not a word." << std::endl;
            }
            else if(!copyFoundWords->contains(possibleWord))
            {
                std::cout << possibleWord << " is not on board." << std::endl;
            }
            else if(!((player == 1)?foundWords:player2Words)->contains(possibleWord))
            {
                std::cout << possibleWord << " is already found." << std::endl;
            }
            else
            {
                std::cout << "Correct." << std::endl;
                if(player == 1)
                {
                    firstPlayerScore += possibleWord.size() - 3;
                    foundWords->remove(possibleWord);
                }
                else
                {
                    secondPlayerScore += possibleWord.size() - 3;
                    player2Words->remove(possibleWord);
                }
            }
        }
    }

    void gameSummary()
    {
        std::cout << "Player 1 Score: " << firstPlayerScore << std::endl;
        std::cout << "Player 2 Score: " << secondPlayerScore << std::endl;
        if(firstPlayerScore > secondPlayerScore)
        {
            std::cout << "Player 1 wins!" << std::endl;
        }
        else if(firstPlayerScore < secondPlayerScore)
        {
            std::cout << "Player 2 wins!" << std::endl;
        }
        else
        {
            std::cout << "It's a tie!" << std::endl;
        }
        std::cout << "All Possible Words: ";
        copyFoundWords->mapAll(printWord);
    }
    void startGame()
    {
        initMap();
        initWordList();
        gameProcess(1);
        gameProcess(2);
        gameSummary();
    }
};

int main() {
  // TODO
  mainGame * game = new mainGame();
  game->startGame();
//    system("pause");
  return 0;
}
