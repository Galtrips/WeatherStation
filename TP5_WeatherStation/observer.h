#ifndef OBSERVER_H
#define OBSERVER_H

/**
 * @brief The Observer class
 */
class Observer {
public:
  virtual ~Observer() =default;
  virtual void update() = 0;
};

/**
 * @brief The Observable class
 */
class Observable {
public:
  virtual ~Observable() =default;
  virtual void addObserver(Observer* observer) = 0;
  virtual void removeObserver(Observer* observer) = 0;
  virtual void notifyObserver() = 0;
};

#endif // OBSERVER_H
