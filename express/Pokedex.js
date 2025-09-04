const express = require('express');
const expressLayouts = require('express-ejs-layouts');
const path = require('path');
const axios = require('axios');
const http = require('http');
const https = require('https');

const app = express();
const port = process.env.PORT || 3000;

const api = axios.create({
  baseURL: process.env.BACKEND_BASE_URL || 'http://localhost:8080',
  timeout: 15000, // 15s per attempt
  httpAgent: new http.Agent({ keepAlive: true }),
  httpsAgent: new https.Agent({ keepAlive: true })
});

async function getWithRetry(url, { params } = {}, label = url, {
  retries = 6,             // ~ total ~14–30s worst-case depending on jitter
  baseDelayMs = 500,       // starting backoff
  maxDelayMs = 4000,       // cap per-try delay
} = {}) {
  let attempt = 0;

  const shouldRetry = (err) => {
    if (err.code === 'ECONNABORTED' || err.code === 'ECONNREFUSED' || err.code === 'ENOTFOUND') return true; // socket/timeouts
    if (!err.response) return true;                 // no response => network-ish
    const { status } = err.response;
    return status >= 500 || status === 429;         // server busy/down or rate-limited
  };

  for (;;) {
    try {
      return await api.get(url, { params });
    } catch (err) {
      attempt += 1;
      if (attempt > retries || !shouldRetry(err)) {
        throw augmentAxiosError(err, { label, attempt });
      }
      const backoff = Math.min(maxDelayMs, baseDelayMs * 2 ** (attempt - 1));
      const jitter = Math.floor(Math.random() * 250);
      const sleep = backoff + jitter;
      console.warn(`[retry] ${label} failed (attempt ${attempt}/${retries}). Retrying in ${sleep}ms…`);
      await new Promise((r) => setTimeout(r, sleep));
    }
  }
}

function augmentAxiosError(err, { label, attempt }) {
  err._context = { label, attempt };
  return err;
}

function logError(err, contextMsg = '') {
  const safe = {
    message: err?.message,
    name: err?.name,
    code: err?.code,
    status: err?.response?.status,
    url: err?.config?.url,
    ctx: err?._context,
  };
  console.error(contextMsg, safe);
}

/* ---------------------------- Express set-up ---------------------------- */
app.set('view engine', 'ejs'); // Set EJS as the view engine
app.set('views', path.join(__dirname)); // Set the views directory to the current folder
app.use(expressLayouts); // Use express-ejs-layouts for layout management
app.set('layout', path.join(__dirname, 'layouts', 'main.ejs')); // Set the layout file, assuming 'main.ejs' is in the 'layouts' folder
app.use(express.json());

// Serve static files from the "public" folder (icons, css, etc.)
app.use('/css', express.static(path.join(__dirname, 'public', 'css')));
app.use('/icons', express.static(path.join(__dirname, 'public', 'icons')));

/* -------------------------------- Routes -------------------------------- */

// GET /
app.get('/', async (req, res) => {
  try {
    const regionNamesResp = await getWithRetry('/getRegionData', {}, 'getRegionData');
    const latestPokemonResp = await getWithRetry('/getLatestsPokemon', {}, 'getLatestsPokemon');

    const regionNames = regionNamesResp.data || [];
    const latestPokemonData = latestPokemonResp.data || [];
    const startingPokemonData = [];

    for (const region of regionNames) {
        const regionName = region.name;
        try {
            const response = await getWithRetry(`getStarting/${regionName}`, {}, `getStarting/${regionName}`);
            startingPokemonData.push(response.data);
        } catch (error) {
            console.error(`Error fetching starting Pokémon for ${regionName}:`, error);
            startingPokemonData.push(null);
        }
    }

    res.render('Index', {
      regions: regionNames,
      latest: latestPokemonData,
      startingPokemon: startingPokemonData,
    });
  } catch (err) {
    logError(err, 'Error building home page:');
    res.status(500).send('Internal Server Error');
  }
});

// GET /region/:region
app.get('/region/:region', async (req, res) => {
  const region = req.params.region;
  try {
    const response = await getWithRetry(`/region/${encodeURIComponent(region)}`, {}, `region(${region})`);
    res.render('Region', {
      region,
      pokemons: response.data,
    });
  } catch (err) {
    logError(err, `Error fetching region ${region}:`);
    res.status(500).send('Internal Server Error');
  }
});

// GET /pokemon/:pokedex_number/:region_id?
app.get('/pokemon/:pokedex_number/:region_id?', async (req, res) => {
  const pokedexNumber = req.params.pokedex_number;
  const regionId = req.params.region_id;

  try {
    // Fetch in parallel; each call uses its own retry strategy.
    const [pokemonResp, typesResp, gamesResp, mainlineResp] = await Promise.all([
      getWithRetry(`/pokemon/${encodeURIComponent(pokedexNumber)}${regionId ? `/${encodeURIComponent(regionId)}` : ''}`, {}, `pokemon(${pokedexNumber},${regionId ?? 'NA'})`),
      getWithRetry('/getPokemonTypes', {}, 'getPokemonTypes'),
      getWithRetry('/getPokemonGames', {}, 'getPokemonGames'),
      getWithRetry('/getPokemonMainlineGames', {}, 'getPokemonMainlineGames'),
    ]);

    res.render('Pokemon', {
      pokemon: pokemonResp.data,
      pokemonTypes: typesResp.data,
      pokemonGames: gamesResp.data,
      pokemonMainlineGames: mainlineResp.data,
    });
  } catch (err) {
    logError(err, `Error fetching pokemon ${pokedexNumber}:`);
    res.status(500).send('Internal Server Error');
  }
});

// GET /search
app.get('/search', async (req, res) => {
  try {
    const searchTerm = (req.query.searchTerm || '').trim();

    if (searchTerm === '') {
      return res.render('Search', { pokemons: '', searchTerm });
    }

    const response = await getWithRetry(`/search`, { params: { searchTerm } }, `search/${searchTerm}`, {
      retries: 4,
      baseDelayMs: 300,
      maxDelayMs: 2000,
    });

    res.render('Search', {
      pokemons: response.data,
      searchTerm,
    });
  } catch (err) {
    logError(err, 'Error in search route:');
    res.status(500).send('Internal Server Error');
  }
});

// GET /logs
app.get('/logs', async (req, res) => {
  try {
    const response = await getWithRetry('/getLogs', {}, 'getLogs');
    res.render('Logs', {
      logs: response.data,
    });
  } catch (err) {
    logError(err, 'Error fetching logs:');
    res.status(500).send('Internal Server Error');
  }
});

/* ------------------------------ Start server ---------------------------- */
app.listen(port, '0.0.0.0', () => {
  console.log(`Server running at http://localhost:${port}`);
});
