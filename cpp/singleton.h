#ifndef _SINGLETON_H
#define _SINGLETON_H

template <typename C>
class FactorySingleton {
public:
  template <typename TDerived>
  static bool _create() {
    if (_pInstance_)
      return false;

    _pInstance_ = new TDerived();

    return true;
  }

  static bool _create() {
    return _create<C>();
  }

  static bool _destroy() {
    if (_pInstance_) {
      delete _pInstance_;
      _pInstance_ = nullptr;
    }

    return true;
  }

  static C* _instance() {
    return _pInstance_;
  }
  virtual ~FactorySingleton() { }

private:
  static C* _pInstance_;

protected:
  FactorySingleton() { }
};

template <typename C> C* FactorySingleton <C>::_pInstance_ = nullptr;

#endif // !_SINGLETON_H
