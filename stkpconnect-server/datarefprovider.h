#ifndef DATAREFPROVIDER_H
#define DATAREFPROVIDER_H
#include <QString>

class DataRef;

// Command types used with command()
enum stkpconnectCommandType {
    stkpconnectCommandTypeInvalid = 0,
    stkpconnectCommandTypeOnce,
    stkpconnectCommandTypeBegin,
    stkpconnectCommandTypeEnd,
};

/**
  * Interface class for subscribing and unsubscribing datarefs
  */
class DataRefProvider {
public:
    DataRefProvider();
    virtual ~DataRefProvider();
    // Subscribe to a named dataref and return pointer to a DataRef instance. Must send the
    // initial value of dataref once.
    virtual DataRef *subscribeRef(QString &name)=0;
    // Unsubscribe the given dataref
    virtual void unsubscribeRef(DataRef *ref)=0;
    // Update ref value from simulator
    virtual void updateDataRef(DataRef *ref)=0;
    // Change ref value to simulator
    virtual void changeDataRef(DataRef *ref)=0;
    // Cause a key stroke in simulator (X-Plane keyid)
    virtual void keyStroke(int keyid)=0;
    // Press (and hold) a button (X-Plane button id)
    virtual void buttonPress(int buttonid)=0;
    // Release a button (X-Plane button id)
    virtual void buttonRelease(int buttonid)=0;
    // Run a named command
    virtual void command(QString &name, stkpconnectCommandType type)=0;
    //Load situation file from disk, relatively to simulator root folder
    //Returns true on success
    virtual bool loadSituation(QString sitFileLocation)=0;
    virtual void addFMSEntryLatLon(QString fmsEntryLine)=0;
    virtual void clearAllFmsEntries()=0;
    virtual void setDestinationFmsEntry(int index)=0;
    virtual void move(QString l)=0;
};

#endif // DATAREFPROVIDER_H
