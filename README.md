<h1>Cub3D</h1>
  <p><strong>Cub3D</strong> is a simple first-person 3D game engine inspired by the classic <em>Wolfenstein 3D</em>. Built using <strong>C</strong> and <strong>MiniLibX</strong>, this project demonstrates ray-casting to render a 3D view of a 2D map.</p>

  <h2>Features</h2>
  <ul>
    <li>Real-time 3D rendering using ray-casting</li>
    <li>Navigation via keyboard (W, A, S, D, arrow keys)</li>
    <li>Supports custom wall textures for all directions (NO, SO, EA, WE)</li>
    <li>Floor and ceiling coloring</li>
    <li>Parses <code>.cub</code> map files</li>
    <li>Proper error handling and memory management</li>
  </ul>

  <h2>Requirements</h2>
  <ul>
    <li>GCC compiler</li>
    <li>MiniLibX library</li>
    <li><code>make</code></li>
  </ul>

  <h2>Compilation</h2>
  <pre><code>make</code></pre>

  <h2>Usage</h2>
  <pre><code>./cub3D maps/example.cub</code></pre>

  <h2>Controls</h2>
  <ul>
    <li><strong>W/A/S/D</strong> – Move forward/left/backward/right</li>
    <li><strong>Arrow keys</strong> – Look left/right</li>
    <li><strong>ESC</strong> – Exit the program</li>
  </ul>

  <h2>File Format (.cub)</h2>
  <p>The <code>.cub</code> file must contain:</p>
  <ul>
    <li>Texture paths (NO, SO, WE, EA)</li>
    <li>Floor and ceiling colors (F, C)</li>
    <li>A map made of <code>1</code> (walls), <code>0</code> (empty space), and one player start (N/S/E/W)</li>
  </ul>

  <h2>Bonus (if implemented)</h2>
  <ul>
    <li>Wall collisions</li>
    <li>Minimap</li>
    <li>Doors</li>
    <li>Sprites</li>
    <li>Mouse look</li>
  </ul>

