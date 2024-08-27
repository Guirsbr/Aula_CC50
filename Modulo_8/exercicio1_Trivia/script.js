function respostaME(numBotao, numQuestao, respostaCorreta)
{
    for (let i = 1; i < 5; i++) {
        let botao = document.querySelector("#botao" + i + "questao" + numQuestao);
        botao.style.backgroundColor = "#d9edff";
    }
    let botao = document.querySelector("#botao" + numBotao + "questao" + numQuestao);
    let texto = document.querySelector("#textoRespostaIncorretoME" + numQuestao);
    if (respostaCorreta) {
        botao.style.backgroundColor = "#b4d3b2";
        texto.style.color = "#b4d3b2";
        texto.innerHTML = "Resposta Correta!";
    } else {
        botao.style.backgroundColor = "#ff6961";
        texto.style.color = "#ff6961";
        texto.innerHTML = "Resposta Incorreta.";
    }
}

function respostaRE(numQuestao)
{
    let resposta = document.querySelector('#inputRespostaRE' + numQuestao).value.toLowerCase();
    let texto = document.querySelector("#textoRespostaIncorretoRE" + numQuestao);

    switch (numQuestao) {
    case 1:
        if (resposta == "hr") {
            texto.style.color = "#b4d3b2";
            texto.innerHTML = "Resposta Correta!";
        } else {
            texto.style.color = "#ff6961";
            texto.innerHTML = "Resposta Incorreta.";
        }
        break;
    case 2:
        if (resposta == "br") {
            texto.style.color = "#b4d3b2";
            texto.innerHTML = "Resposta Correta!";
        } else {
            texto.style.color = "#ff6961";
            texto.innerHTML = "Resposta Incorreta.";
        }
        break;
    default:
        break;
    }
}