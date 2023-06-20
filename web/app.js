
function getValue(value) {
    // Sélectionner l'élément input et récupérer sa valeur
    var input = value;

    let url = "http://10.59.2.240:4000/inputs/0/";
    url = url.concat(input);

    const xhr = new XMLHttpRequest();
    xhr.open("GET", url);
    xhr.setRequestHeader("Access-Control-Allow-Origin", "*");
    xhr.send();
    xhr.responseType = "json";
    xhr.onload = () => {
    if (xhr.readyState == 4 && xhr.status == 200) {
        const data = xhr.response;
        console.log(data);
    } else {
        console.log(`Error: ${xhr.status}`);
    }
    };
}


