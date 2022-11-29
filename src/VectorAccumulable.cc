void VectorAccumulable::Merge(const G4VAccumulable& other)
{
  const VectorAccumulable& otherVectorAccumulable
    = static_cast<const VectorAccumulable&>(other);

  for (uint i = 0;i<fProcCounter.size(); ++i) {
         fVector.at(i) += otherCount;
  };
}

void ProcCounterAccumulable::Reset()
{
  fVector.clear();
}
