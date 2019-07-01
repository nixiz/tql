#pragma once

/*
* Template Recursion i�lemlerini bir yerde k�rmak gerekiyor.
* Typelist s�n�f�n� sonsuz d�ng�den kurtarmak i�in null_type(ismi �nemli de�il)
* gibi �nceden belirlenecek bir tip �zerinden differentiate edilmesi gerekiyor.
* �rne�in dosyalar�n EOF belirtecinin olmas� gibi.
*/
class null_type {};

/// TODO(oguzhank): add various preprocessor configuration support. 
/// like both C++ 98 and 11/14/17 support and 
/// POC(test) builds in code.

// #ifdef BUILD_POC