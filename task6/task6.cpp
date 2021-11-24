#include <fstream>
#include <iostream>
/* test2.c */
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

    for (int i = 0; i < pageframes.MAXIMUM_SIZE; i++)
    {
        if (pageframes.frames[i].start <= address && address <= pageframes.frames[i].end)
        {
            return true;
        }
    }
    return false;
}

void LRU(pageFrames &pageframes, int start, int end)
{
    pageframes.frames[0].start = start;
    pageframes.frames[0].end = end;
}

void shiftMostRecentlyUsed(pageFrames &pageframes, int start, int end)
{
    int tempStart = 0;
    int tempEnd = 0;
    int pageIndex = 0;
    //std::cout << "Started shifting function..." << std::endl;
    // find most recently used page index
    for (int i = 0; i < pageframes.size; i++) {
        if (pageframes.frames[i].start == start && pageframes.frames[i].end == end) {
            pageIndex = i;
            //std::cout << "found index: " << pageIndex << std::endl;
            break;
        }
    }
    // if page is already the most recently used, do nothing and return.
    if(pageIndex == pageframes.size - 1){
        //std::cout << "Returning early..." << std::endl;
        return;
    }
    // If not most recently used,
    // shift everything to the right of pageIndex left one step.
    //std::cout << "Started shifting loop..." << std::endl;
    for (int i = pageIndex; i < pageframes.size - 1; i++)
    {
    //std::cout << "Shifting..." << std::endl;
        pageframes.frames[i].start = pageframes.frames[i + 1].start;
        pageframes.frames[i].end = pageframes.frames[i + 1].end;
    }
    // insert last used page in last position.
    if (pageframes.size == pageframes.MAXIMUM_SIZE) {
        pageframes.frames[pageframes.MAXIMUM_SIZE - 1].start = start;
        pageframes.frames[pageframes.MAXIMUM_SIZE - 1].end = end;
    }else{
        pageframes.frames[pageframes.size].start = start;
        pageframes.frames[pageframes.size].end = end;
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
        int results[2] = {0, 0};
        while (std::getline(memFile, line))
        {
            getInterval(std::stoi(line), pageSize, results);
            //std::cout << "Interval: [" << results[0] <<" " << results[1] << "]" << std::endl;
            if (pageframes.size < MAXIMUM_PAGES && !isInPagesFrames(pageframes, std::stoi(line)))
            {
                if (pageframes.size == 0)
                {
                    pageframes.frames[0].start = results[0];
                    pageframes.frames[0].end = results[1];
                }
                else
                {
                    for (int i = 0;i < pageframes.size - 1;i++)
                    {
                        pageframes.frames[i].start = pageframes.frames[i + 1].start;
                        pageframes.frames[i].end = pageframes.frames[i + 1].end;
                    }
                        pageframes.frames[0].start = results[0];
                        pageframes.frames[0].end = results[1];
                }
                pagesFaults++;
                //std::cout << "Pagefault!\n";
                pageframes.size++;
            }
            else if (!isInPagesFrames(pageframes, std::stoi(line)))
            {
                LRU(pageframes, results[0], results[1]);
                //std::cout << "Pagefault!\n";
                pagesFaults++;
            }
            else if (pageframes.size >= MAXIMUM_PAGES)
            {
                shiftMostRecentlyUsed(pageframes, results[0], results[1]);
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
