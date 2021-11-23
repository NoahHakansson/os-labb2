#include <fstream>
#include <iostream>

struct page
{
    int start;
    int end;
    page *next;
    page()
    {
        start = 0;
        end = 0;
        next = nullptr;
    }
    page(int start, int end)
    {
        this->start = start;
        this->end = end;
        next = nullptr;
    }
};

struct pageFrames
{
    page *start;
    int size;
    pageFrames()
    {
        size = 0;
        start = nullptr;
    }
};

void getInterval(int address, int const interval, int *values)
{
    int start;
    int end;
    int i = 0;
    while ((interval * i) <= address)
    {
        start = interval * i;
        i++;
    }
    end = (interval * i) - 1;
    int returnValues[2];
    values[0] = start;
    values[1] = end;
}

bool isInPagesFrames(pageFrames pageframes, int address)
{
    page *walker = pageframes.start;
    while (walker != nullptr)
    {
        if (walker->start <= address && walker->end >= address)
        {
            return true;
        }
        walker = walker->next;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        int const MAXIMUM_PAGES = std::stoi(argv[1]);
        int pagesFaults = 0;
        int linesRead = 0;
        pageFrames pageframes;
        std::string fileName = argv[3];
        int const pageSize = std::stoi(argv[2]);
        int const interval = ((pageSize * 8)) / 32;

        std::ifstream memFile(fileName);
        std::string line;
        int results[2] = {0, 0};
        while (std::getline(memFile, line))
        {
            if (!isInPagesFrames(pageframes, std::stoi(line)))
            {
                getInterval(std::stoi(line), interval, results);
                if (pageframes.size < MAXIMUM_PAGES)
                {
                    if (pageframes.size == 0)
                    {
                        pageframes.start = new page(results[0], results[1]);
                    }
                    else
                    {
                        page *walker = pageframes.start;
                        while (walker != nullptr && pageframes.size != 0)
                        {
                            walker = walker->next;
                        }
                        walker = new page(results[0], results[1]);
                    }
                    pageframes.size++;
                }
                else
                {
                    pageframes.start->start = results[0];
                    pageframes.start->end = results[1];
                }
                pagesFaults++;
            }
            linesRead++;
        }
        memFile.close();
        std::cout << "No physical pages = " << MAXIMUM_PAGES << ", page size = " << pageSize << std::endl;
        std::cout << "Reading memory trace from " << fileName << std::endl;
        std::cout << "Read " << linesRead << " memory references => " << pagesFaults << " pagefaults" << std::endl;
        std::cout << "Page frames: " << pageframes.size << std::endl;
    }
    else
    {
        std::cout << "Wrong amount of arguments!\nArguments written: " << argc << std::endl;
        return 0;
    }
}
