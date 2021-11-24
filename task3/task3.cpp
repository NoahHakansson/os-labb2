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
    page *frames;
    int size;
    int MAXIMUM_SIZE;
    pageFrames()
    {
        size = 0;
        MAXIMUM_SIZE = 0;
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

    for (int i = 0;i < pageframes.MAXIMUM_SIZE;i++)
    {
        if (pageframes.frames[i].start <= address && address <= pageframes.frames[i].end)
        {
            return true;
        }
    }
    return false;
}

void FIFO(pageFrames &pageframes, int start, int end)
{
    for (int i = 0;i < pageframes.MAXIMUM_SIZE - 1;i++)
    {
        pageframes.frames[i].start = pageframes.frames[i + 1].start;
        pageframes.frames[i].end = pageframes.frames[i + 1].end;
    }
    pageframes.frames[pageframes.MAXIMUM_SIZE - 1].start = start;
    pageframes.frames[pageframes.MAXIMUM_SIZE - 1].end = end;
}

void writePageFrames(pageFrames pageframes)
{
    for (int i = 0;i < pageframes.MAXIMUM_SIZE;i++)
    {
        std::cout << "{" << pageframes.frames[i].start << ", " << pageframes.frames[i].end << "} ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 4)
    {
        int const MAXIMUM_PAGES = std::stoi(argv[1]);
        int pagesFaults = 0;
        int linesRead = 0;
        std::string fileName = argv[3];
        int const pageSize = std::stoi(argv[2]);
        int const interval = pageSize / 1;

        pageFrames pageframes;
        pageframes.MAXIMUM_SIZE = MAXIMUM_PAGES;
        page frames[MAXIMUM_PAGES];
        pageframes.frames = frames;

        std::ifstream memFile(fileName);
        std::string line;
        int results[2] = {0, 0};
        while (std::getline(memFile, line))
        {
            if (pageframes.size < MAXIMUM_PAGES && !isInPagesFrames(pageframes, std::stoi(line)))
            {
                getInterval(std::stoi(line), interval, results);
                if (pageframes.size == 0)
                {
                    pageframes.frames[0].start = results[0];
                    pageframes.frames[0].end = results[1];
                }
                else
                {
                    pageframes.frames[pageframes.size].start = results[0];
                    pageframes.frames[pageframes.size].end = results[1];
                }
                pagesFaults++;
                pageframes.size++;
                std::cout << "Pagefault!\n";
            }
            else if (!isInPagesFrames(pageframes, std::stoi(line)))
            {
                getInterval(std::stoi(line), interval, results);

                FIFO(pageframes, results[0], results[1]);

                pagesFaults++;
                std::cout << "Pagefault!\n";
            }
            writePageFrames(pageframes);
            linesRead++;
        }
        memFile.close();
        std::cout << "No physical pages = " << MAXIMUM_PAGES << ", page size = " << pageSize << std::endl;
        std::cout << "Reading memory trace from " << fileName << std::endl;
        std::cout << "Read " << linesRead << " memory references => " << pagesFaults << " pagefaults" << std::endl;
        std::cout << "Interval: " << interval << std::endl;
        std::cout << "array size: " << pageframes.size << std::endl;
    }
    else
    {
        std::cout << "Wrong amount of arguments!\nArguments written: " << argc << std::endl;
        return 0;
    }
}
