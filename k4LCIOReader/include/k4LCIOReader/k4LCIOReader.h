#ifndef k4_LCIOREADER_H
#define k4_LCIOREADER_H

#include "podio/IReader.h"
#include "podio/CollectionIDTable.h"
#include <map>

/// forward declarations
// in LCIO
namespace IO
{
class LCReader;
}
// in podio
namespace podio
{
class EventStore;
class GenericParameters;
}
// others
class k4LCIOConverter;

class k4LCIOReader : public podio::IReader
{
    friend podio::EventStore;

public:
    k4LCIOReader();
    ~k4LCIOReader();

    void openFile(const std::string &filename);
    void openFiles(const std::vector<std::string> &filenames);
    void closeFile();
    void closeFiles();

    void setReadCollectionNames(const std::vector<std::string> &colnames);

    /// Prepare LCCollections requested
    bool readNextEvent();

    /// get an edm4hep collection
    template <typename T>
    T *getCollection(const std::string &name);

    /// Read CollectionIDTable from ROOT file
    podio::CollectionIDTable *getCollectionIDTable() override final { return m_table; }

    /// Returns number of entries in the file
    unsigned getEntries() const { return m_entries; }

    /// Preparing to read next event
    //void endOfEvent();

    /// Preparing to read a given event
    //void goToEvent(unsigned evnum);

    /// Check if TFile is valid
    virtual bool isValid() const override final;

    /// pure virtual methods that from parent class
    virtual podio::GenericParameters* readEventMetaData();
    virtual std::map<int, podio::GenericParameters>* readCollectionMetaData();
    virtual std::map<int, podio::GenericParameters>* readRunMetaData();
    virtual void endOfEvent();

    podio::version::Version currentFileVersion() const override { return m_fileVersion; }

private:
    /// Implementation for collection reading
    podio::CollectionBase *readCollection(const std::string &name) override final;

private:
    unsigned m_entries;

    //LCReader in LCIO
    IO::LCReader *m_reader;

    //the converter
    k4LCIOConverter *m_converter;

    ///...
    podio::CollectionIDTable *m_table;

    podio::version::Version m_fileVersion{0, 0, 0};
};

template <typename T>
T *k4LCIOReader::getCollection(const std::string &name)
{
    auto p = dynamic_cast<T *>(readCollection(name));
    if (p == nullptr)
    {
        p = new T();
        p->setID(m_table->add(name));
    }
    return p;
}

#endif
