#ifndef Accumulable_hh
#define Accumulable_hh 1

#include "G4VAccumulable.hh"
#include "globals.hh"
#include <map>
class VectorAccumulable : public G4VAccumulable
{
  public:
    VectorAccumulable(const G4String& name)
      : G4VAccumulable(name, 0), fVector() {}
    virtual ~VectorAccumulable() {}

    void CountProcesses(G4String vecName);

    virtual void Merge(const G4VAccumulable& other);
    virtual void Reset();

  private:
    std::vector<G4double> fVector;
};

#endif
