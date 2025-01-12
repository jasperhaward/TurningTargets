import logo from "./assets/images/logo.png";
import styles from "./App.module.scss";

export default function App() {
  return (
    <div>
      <div className={styles.brand}>
        <img height={40} width={40} src={logo} />
        <h1>RMRPC Turning Targets</h1>
      </div>
    </div>
  );
}
