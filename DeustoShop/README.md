# 🛒 DeustoShop  
> Proyecto completo de una **tienda online**, desarrollado en C y C++. Arquitectura Cliente – Servidor – Administrador.


[![Lenguajes](https://img.shields.io/badge/C-40.9%25-05599C?logo=c)]()
[![Lenguajes](https://img.shields.io/badge/C++-59.1%25-00599C?logo=c%2B%2B)]()

---

## 📌 Resumen

**DeustoShop** es un proyecto que simula el funcionamiento de una tienda online real, con dos
aplicaciones independientes (cliente y administrador), conectadas
mediante un servidor central que gestiona toda la lógica de comunicación.

---

## ✨ Características Principales

### 👤 Cliente (C)
- Registro e inicio de sesión
- Exploración y búsqueda de productos
- Carrito de compras y confirmación de pedidos
- Historial de pedidos y seguimiento
- Manejo básico de errores y reconexión

### 🖥️ Servidor (C/C++)
- Autenticación y sesión de usuarios
- Gestión de catálogo y stock
- Procesamiento de pedidos y actualización de inventario
- Comunicación bidireccional (clientes ↔ administrador)
- Persistencia de datos (archivos / posible extensión a SQLite)
- Registro (logging) de eventos y auditoría

### 🛠️ Administrador (C++)
- Alta, baja y modificación de productos
- Gestión de proveedores y stock mínimo
- Visualización de métricas (ventas, pedidos, actividad)
- Panel para inspeccionar pedidos en tiempo real
- Herramientas de mantenimiento (resincronización, backup)

---

## 📂 Estructura de Directorios

``` plaintext
DeustoShop/
├── README.md
├── DeustoShopC/             # Cliente (C)
│   └── cliente/
├── DeustoShopCpp/           # Admin y Servidor (C++)
│   ├── admin/
│   └── servidor/
└── otros_archivos/
```

---

## ⚙️ Ejecución

```bash
# Servidor
cd DeustoShopCpp/
./Servidor.exe

# Cliente
cd ../../DeustoShopC/
./DeustoShop.exe

# Administrador
cd ../../DeustoShopC++/
./DeustoShopCpp.exe
```

---