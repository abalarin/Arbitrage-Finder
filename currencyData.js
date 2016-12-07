$(document).ready(function() {
  //var jax = $.getJSON('http://api.fixer.io/latest');

  var c1 = apiCall('EUR');
  makeLi(c1, 'c1', 'EUR');
  var c2 = apiCall('USD');
  makeLi(c2, 'c2', 'USD');
  var c3 = apiCall('CAD');
  makeLi(c3, 'c3', 'CAD');
  var c4 = apiCall('GBP');
  makeLi(c4, 'c4', 'GBP');

  function makeLi(jax, str, den){
    jax.done(function(data){
      c = data.rates;
      console.log(String('.' + str));
      $('<li>').text(den).addClass('liTitle').appendTo('.' + str);
      for (key in c) {
        $('<li>')
            .text(key + ' : ' + c[key])
            .addClass('currencyData')
            .appendTo('.' + str);
      }
    })
  }

  function apiCall(currency){
    var jax = $.getJSON('http://api.fixer.io/latest?base=' + currency);
    return jax.done(function(data){
      return data.rates;
    })
  }
});
