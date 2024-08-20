var collapseElementList = [].slice.call(document.querySelectorAll('.collapse'))
var collapseList = collapseElementList.map(function (collapseEl) {
  return new bootstrap.Collapse(collapseEl, {
    toggle: false
  })
})

function ativarCollapse(botao)
{
    let listaBotoes = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

    if (botao == 0 || botao == 1 || botao == 2)
    {
        listaBotoes = listaBotoes.map((num) => {
            if (num != 0 && num != 1 && num != 2) {
                return num;
            }
        });
    }
    else if (botao == 3 || botao == 4 || botao == 5)
    {
        listaBotoes = listaBotoes.map((num) => {
            if (num != 3 && num != 4 && num != 5) {
                return num;
            }
        });
    }
    else if (botao == 6 || botao == 7)
    {
        listaBotoes = listaBotoes.map((num) => {
            if (num != 6 && num != 7) {
                return num;
            }
        });
    }
    else if (botao == 8 || botao == 9)
    {
        listaBotoes = listaBotoes.map((num) => {
            if (num != 8 && num != 9) {
                return num;
            }
        });
    }

    listaBotoes.map((num) => {
        if (num != undefined)
        {
            collapseList[num].hide();
        }
    });

    collapseList[botao].toggle();
}

function max_width_768px()
{
    if (window.innerWidth <= 768)
    {
        let rowElementList = [].slice.call(document.querySelectorAll('.rowRemove'))
        let colElementList = [].slice.call(document.querySelectorAll('.colRemove'))
        rowElementList.map((element) => {
            element.classList.remove("row");
            
        })
        colElementList.map((element) => {
            element.classList.remove("col");
            element.classList.remove("col-9");
            element.classList.remove("col-3");
        });

    }
}
