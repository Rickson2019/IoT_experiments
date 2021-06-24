let btn_update_sta = document.getElementById("update_sta")



btn_update_sta.addEventListener("click",()=>{
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = () => {
      if (this.readyState == 4 && this.status == 200) {
        btn_update_sta.innerHTML = this.responseText;
      }
    };
    xhttp.open("POST", "/update_sta", true);
    xhttp.send();
})