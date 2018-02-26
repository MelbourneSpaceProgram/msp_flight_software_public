#ifndef SRC_SENSORS_READING_H_
#define SRC_SENSORS_READING_H_

template <typename T>
class Reading {
   public:
    T GetReading() const { return reading; }

   protected:
    T reading;
};

#endif  //  SRC_SENSORS_READING_H_
