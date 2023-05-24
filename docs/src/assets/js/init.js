document.addEventListener('DOMContentLoaded',init);

function init() {
    addHeader();
}

function addHeader(){
    const $header = document.querySelector('header');
    $header.insertAdjacentHTML('afterBegin',header);
}