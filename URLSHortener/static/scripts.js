function p_check(){
    var element = document.querySelector('.forma p');
    var userURL = element.textContent.trim();

    if (userURL.length > 30) {
     element.textContent = "User URL: " + userURL.substring(0, 30) + "...";
    }
}
