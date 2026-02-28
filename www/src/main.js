async function loadDefinitions() {
  try {
    const res = await fetch("/definitions");
    const data = await res.json();
    buildUI(data.definitions);
  } catch (err) {
    console.error("Erro ao carregar defs:", err);
  }
}

function sendValue(id, value) {
  const body = `id=${id}\nvalue=${value}`;
  fetch("/variable", {
    method: "POST",
    headers: { "Content-Type": "text/plain" },
    body
  }).catch((err) => console.error(err));
}

function sendAction(id) {
  const params = new URLSearchParams().append("id", id);
  const url = `/action${params}`;

  fetch(url, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
  }).catch((err) => console.error(err));
}

function buildUI(defs) {
  const container = document.getElementById("controls");
  container.innerHTML = "";

  defs.forEach((def) => {
    const div = document.createElement("div");
    div.className = "control";

    const label = document.createElement("label");
    label.innerText = def.name;
    div.appendChild(label);

    if (def.selector === "text") {
      const input = document.createElement("input");
      input.type = "number";
      input.value = def.value ?? 0;

      input.onchange = () => {
        let v = input.value;
        if (def.type === "int") v = parseInt(v);
        if (def.type === "float") v = parseFloat(v);
        sendValue(def.id, v);
      };

      div.appendChild(input);
    } else if (def.selector === "slider") {
      const input = document.createElement("input");
      input.type = "range";
      input.min = def.min ?? 0;
      input.max = def.max ?? 100;
      input.step = def.type === "float" ? 0.01 : 1;
      input.value = def.value ?? 0;

      const valueLabel = document.createElement("div");
      valueLabel.className = "sliderValue";
      valueLabel.innerText = input.value;

      input.oninput = () => {
        valueLabel.innerText = input.value;
      };

      input.onchange = () => {
        let v = input.value;
        if (def.type === "int") v = parseInt(v);
        if (def.type === "float") v = parseFloat(v);
        sendValue(def.id, v);
      };

      div.appendChild(input);
      div.appendChild(valueLabel);
    } else if (def.selector === "checkbox") {
      const input = document.createElement("input");
      input.type = "checkbox";
      input.checked = def.value ?? false;

      input.onchange = () => {
        sendValue(def.id, input.checked);
      };

      div.appendChild(input);
    } else if (def.type === "action") {
      const button = document.createElement("button");
      button.innerText = def.id;

      button.onclick = () => {
        sendAction(def.id);
      };

      div.appendChild(button);
    }

    container.appendChild(div);
  });
}

loadDefinitions();
