#ifndef TTLCONDTRIG_TEMPLATES_DEFINED
#define TTLCONDTRIG_TEMPLATES_DEFINED


// C++ compiles templated classes on-demand. The source code has to be included by every file that instantiates templates.
// Only one copy of each variant will actually end up in the binary; extra copies get pruned at link-time.
// There are other ways to handle this, but they're just as ugly as this way.


using namespace TTLConditionTrig;


//
// Circular buffer helper class.

// NOTE - If you store pointers, do your own de-allocation! This doesn't do it for you.

template <class datatype_t,size_t bufsize>
CircBuf<datatype_t,bufsize>::CircBuf()
{
    clear();
}


template <class datatype_t,size_t bufsize>
void CircBuf<datatype_t,bufsize>::clear()
{
    readPtr = 0;
    writePtr = 0;
    dataCount = 0;
}


template <class datatype_t,size_t bufsize>
void CircBuf<datatype_t,bufsize>::enqueue(datatype_t newVal)
{
    // Silently discard this if we're out of space.
    if (dataCount < bufsize)
    {
        dataBuffer[writePtr] = newVal;
        // The compiler will optimize this if bufsize is a power of two.
        writePtr = (writePtr + 1) % bufsize;
        dataCount++;
    }
}


template <class datatype_t,size_t bufsize>
datatype_t CircBuf<datatype_t,bufsize>::dequeue()
{
    // Pick a safe default value.
    datatype_t returnVal = (datatype_t) 0;

    // Only fetch data if there's data to fetch.
    if (dataCount > 0)
    {
        returnVal = dataBuffer[readPtr];
        // The compiler will optimize this if bufsize is a power of two.
        readPtr = (readPtr + 1) % bufsize;
        dataCount--;
    }

    return returnVal;
}


template <class datatype_t,size_t bufsize>
size_t CircBuf<datatype_t,bufsize>::count()
{
    return dataCount;
}


#endif

//
// This is the end of the file.
