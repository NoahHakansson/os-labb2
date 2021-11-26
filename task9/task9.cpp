#include <fstream>
#include <iostream>
#include <vector>
/* test2.c */
struct page
{
    int start;
    int end;
    page *next;
    bool found;
    page()
    {
        start = 0;
        end = 0;
        next = nullptr;
        found = false;
    }
    page(int start, int end)
    {
        this->start = start;
        this->end = end;
        next = nullptr;
        found = false;
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

    for (int i = 0; i < pageframes.MAXIMUM_SIZE; i++)
    {
        if (pageframes.frames[i].start <= address && address <= pageframes.frames[i].end)
        {
            return true;
        }
    }
    return false;
}

int getIndex(pageFrames pageframes, int address)
{
    for (int i = 0; i < pageframes.MAXIMUM_SIZE; i++)
    {
        if (pageframes.frames[i].start <= address && address <= pageframes.frames[i].end)
        {
            return i;
        }
    }
    return -1;
}

bool isInPagesFrames(pageFrames pageframes, int start, int end)
{

    for (int i = 0; i < pageframes.MAXIMUM_SIZE; i++)
    {
        if (pageframes.frames[i].start == start && end == pageframes.frames[i].end)
        {
            return true;
        }
    }
    return false;
}

void optimal(pageFrames &pageframes, int start, int end, int index, std::vector<int> addresses)
{
    int count = 0;
    int pageIndex = 0;
    //Loops through future address calls and sets found to the addresses already existing in page frames, stopping right before the last page frame is found
    for (int i = index + 1;i < addresses.size() && count < pageframes.MAXIMUM_SIZE - 1;i++)
    {
        if (isInPagesFrames(pageframes, addresses[i]))
        {
            pageIndex = getIndex(pageframes, addresses[i]);
            if (pageframes.frames[pageIndex].found == false)
            {
                pageframes.frames[pageIndex].found = true;
                count++;
            }
        }
    }
    //Finds the page frame in the array which is last in the future calls
    //Inserts the new page at the given position
    for (int i = 0;i < pageframes.MAXIMUM_SIZE;i++)
    {
        if (pageframes.frames[i].found == false) 
        {
            pageframes.frames[i].start = start;
            pageframes.frames[i].end = end;
        }
    }
    //Resets all pages for next time optimal() is called
    for (int i = 0;i < pageframes.MAXIMUM_SIZE;i++)
    {
        pageframes.frames[i].found = false;
    }
}

void writePageFrames(pageFrames pageframes)
{
    for (int i = 0; i < pageframes.MAXIMUM_SIZE; i++)
    {
        //std::cout << "{" << pageframes.frames[i].start << ", " << pageframes.frames[i].end << "} ";
    }
    //std::cout << std::endl;
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

        pageFrames pageframes;
        pageframes.MAXIMUM_SIZE = MAXIMUM_PAGES;
        page frames[MAXIMUM_PAGES];
        pageframes.frames = frames;

        std::ifstream memFile(fileName);
        std::string line;
        int results[3] = {0, 0, 0};
        std::vector<int> addresses;
        int value;
        while (std::getline(memFile, line))
        {
            value = std::stoi(line);
	        addresses.push_back(value);
        }
        for (int i = 0; i < addresses.size(); i++)
        {
            getInterval(addresses[i], pageSize, results);
            //std::cout << "Interval: [" << results[0] <<" " << results[1] << "]" << std::endl;
            if (pageframes.size < MAXIMUM_PAGES && !isInPagesFrames(pageframes, addresses[i]))
            {
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
                //std::cout << "Pagefault!\n";
                pageframes.size++;
            }
            else if (!isInPagesFrames(pageframes, addresses[i]))
            {
                optimal(pageframes, results[0], results[1], i, addresses);
                //std::cout << "Pagefault!\n";
                pagesFaults++;
            }
            linesRead++;
            writePageFrames(pageframes);
        }
        memFile.close();
        std::cout << "No physical pages = " << MAXIMUM_PAGES << ", page size = " << pageSize << std::endl;
        std::cout << "Reading memory trace from " << fileName << std::endl;
        std::cout << "Read " << linesRead << " memory references => " << pagesFaults << " pagefaults" << std::endl;
        std::cout << "array size: " << pageframes.size << std::endl;
    }
    else
    {
        std::cout << "Wrong amount of arguments!\nArguments written: " << argc << std::endl;
        return 0;
    }
}
