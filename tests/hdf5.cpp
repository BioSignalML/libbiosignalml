#include "formats/hdf5.h"
#include "model.h"

#include <iostream>
#include <string>
#include <vector>



int main(int argc, char *argv[])
/*----------------------------*/
{

  bsml::HDF5::Recording hdf5("http://ex.org/recording", "test.h5") ;
  hdf5.set_description("testing 123...") ;

  const std::vector<const std::string> uris{"http://ex.org/1",
                                            "http://ex.org/2",
                                            "http://ex.org/3"
                                           } ;
  const std::vector<const std::string> units{"http://units.org/V",
                                             "http://units.org/mV",
                                             "http://units.org/mV"
                                           } ;



  auto *sig = hdf5.new_signal("http://ex.org/signal", units[1]) ;

//  std::cout << hdf5.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;


#define NPOINTS  1001
#define NSIGS       3

//  std::vector<double> times(NPOINTS) ;     // Times as vector<> v's double[]
// C++11  vector.data() is array[] of data elements

  double times[NPOINTS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    times[t] = (double)t/((double)(NPOINTS - 1)) ;
    }

  bsml::HDF5::Clock *clock = hdf5.new_clock("http://ex.org/clock",
                                            "http://units.org/Second",
                                            times, NPOINTS) ;
  clock->set_label("A common clock") ;

  auto signals = hdf5.new_signal(uris, units, clock) ;
  double data[NPOINTS*NSIGS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    for (int n = 0 ;  n < NSIGS ;  ++n) {
      data[t + n] = times[t] ;
      }
    }
  signals.extend(data, sizeof(data)) ;


  hdf5.close() ;  // Should automatically update metadata...


  }


/***

void CsvDataStoreExporter::execute(CoreDataStore::CoreDataStore *pDataStore) const
{
    // Export the given data store to a CSV file

    QString fileName = Core::getSaveFileName(QObject::tr("Export to a CSV file"),
                                             QString(),
                                             QObject::tr("CSV File")+" (*.csv)");

    if (!fileName.isEmpty()) {
        // Header

        static const QString Header = "%1 (%2)";

        CoreDataStore::DataStoreVariable *voi = pDataStore->voi();
        CoreDataStore::DataStoreVariables variables = pDataStore->variables();

        QString data = QString();

        data += Header.arg(voi->uri().replace("/prime", "'").replace("/", " | "),
                           voi->unit());

        auto variableBegin = variables.begin();
        auto variableEnd = variables.end();

        for (auto variable = variableBegin; variable != variableEnd; ++variable)
            data += ","+Header.arg((*variable)->uri().replace("/prime", "'").replace("/", " | "),
                                   (*variable)->unit());

        data += "\n";

        // Data itself

        for (qulonglong i = 0; i < pDataStore->size(); ++i) {
            data += QString::number(voi->value(i));

            for (auto variable = variableBegin; variable != variableEnd; ++variable)
                data += ","+QString::number((*variable)->value(i));

            data += "\n";

            qApp->processEvents();
//---GRY--- THE CALL TO qApp->processEvents() SHOULD BE REMOVED AND THE EXPORTER
//          BE SUCH THAT IT DOESN'T BLOCK THE MAIN THREAD (E.G. WHEN EXPORTING
//          LONG SIMULATIONS). MAYBE THIS COULD BE DONE BY MAKING THE EXPORTER
//          WORK IN ITS OWN THREAD?...
        }

        // The data is ready, so write it to the file

        Core::writeTextToFile(fileName, data);
    }
}



**/
