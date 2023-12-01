// Selecciona el formulario
var form = document.querySelector('form');

// Selecciona el indicador de carga
var loading = document.querySelector('#loading');

// Agrega un event listener para el evento de envío del formulario
form.addEventListener('submit', function(event) {

    // Muestra el indicador de carga
    loading.style.display = 'block';
    
    // Evita que el formulario se envíe de la manera predeterminada
    event.preventDefault();

    // Obtén el texto del input
    var inputText = document.querySelector('#frase').value;

    // Envía la solicitud
    fetch('http://localhost:8080/books/' + inputText, {
        method: 'GET', 
        headers: {
            'Content-Type': 'application/json',
        },
    })
    .then(response => {
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.json();
    })
    .then(json => {
        // Guarda los datos en una variable
        data = json;
        createElements(data);
    })
    .catch(error => console.error('Error:', error));
        // Oculta el indicador de carga
        loading.style.display = 'none';
});

// Itera sobre cada objeto en los datos
function createElements(data) {

    // Oculta el indicador de carga
    loading.style.display = 'none';

    // Selecciona el contenedor donde quieres insertar las secciones
    var container = document.querySelector('.fragments');
    data.forEach(function(item) {
        
        // Crea una nueva sección
        var section = document.createElement('section');
        section.className = 'fragmentSection';

        //Crear una linea horizontal
        var hr = document.createElement('hr');
        section.appendChild(hr);

        var pAuthor = document.createElement('p');
        pAuthor.innerHTML = '<strong>Author</strong>: ' + item.author;
        section.appendChild(pAuthor);

        var pBook = document.createElement('p');
        pBook.innerHTML = '<strong>Book:</strong> ' + item.book;
        section.appendChild(pBook);

        var pPage = document.createElement('p');
        pPage.innerHTML = '<strong>Page</strong>: ' + item.page;
        section.appendChild(pPage);

        // Crea y añade la imagen
        var img = document.createElement('img');
        img.src = item.image;
        img.alt = 'fragmento';
        section.appendChild(img);

        var pFragment = document.createElement('p');
        pFragment.className = 'fragment';
        pFragment.innerHTML = '<strong>Fragment</strong>: ' + item.paragraph;
        section.appendChild(pFragment);

        var pSentiment = document.createElement('p');
        pSentiment.className = 'sentiment';
        pSentiment.innerHTML = '<strong>Sentiment</strong>: ' + item.sentiment;
        section.appendChild(pSentiment);

        // Añade la sección al contenedor
        container.appendChild(section);
    });
}