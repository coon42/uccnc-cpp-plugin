#ifndef _SINGLETON_H
#define _SINGLETON_H

/*
BSD 2-Clause License

Copyright (c) 2018, Stephan Thiele
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
