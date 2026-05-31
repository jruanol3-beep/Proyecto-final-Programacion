-- ============================================================
--   SISTEMA SUPERMERCADO XYZ
--   Script de inserción de datos de prueba
--   Orden respeta las llaves foráneas (FK)
-- ============================================================

-- ============================================================
-- 1. MARCAS  (sin dependencias)
-- ============================================================
INSERT IGNORE INTO marcas (id_marca, marca) VALUES
(1, 'Nestlé'),
(2, 'Coca-Cola'),
(3, 'Colgate'),
(4, 'Unilever'),
(5, 'Bimbo');

-- ============================================================
-- 2. PUESTOS  (sin dependencias)
-- ============================================================
INSERT IGNORE INTO puestos (id_puesto, puesto) VALUES
(1, 'Gerente General'),
(2, 'Cajero'),
(3, 'Bodeguero'),
(4, 'Vendedor'),
(5, 'Supervisor de Piso');

-- ============================================================
-- 3. EMPLEADOS  (depende de puestos)
-- ============================================================
INSERT IGNORE INTO empleados
  (id_empleado, nombres, apellidos, direccion, telefono, genero,
   fecha_nacimiento, id_puesto, fecha_inicio_labores, fecha_ingreso)
VALUES
(1, 'Carlos Alberto',  'Ruano Pérez',    'Zona 1, Guatemala City',      '55551001', 1, '1990-03-15', 1, '2018-01-10', NOW()),
(2, 'María Fernanda',  'López García',   'Zona 5, Guatemala City',      '55551002', 0, '1995-07-22', 2, '2020-05-01', NOW()),
(3, 'Jorge Eduardo',   'Morales Díaz',   'Villa Nueva, Guatemala',      '55551003', 1, '1988-11-30', 3, '2019-03-15', NOW()),
(4, 'Ana Lucía',       'Cifuentes Soto', 'Mixco, Guatemala',            '55551004', 0, '1993-04-18', 4, '2021-08-20', NOW()),
(5, 'Roberto',         'Samayoa Ortiz',  'San Miguel Petapa, Guatemala','55551005', 1, '1985-09-05', 5, '2017-11-11', NOW());

-- ============================================================
-- 4. CLIENTES  (sin dependencias)
-- ============================================================
INSERT IGNORE INTO clientes
  (id_cliente, nombres, apellidos, nit, genero, telefono,
   correo_electronico, fecha_ingreso)
VALUES
(1, 'Luis Miguel',     'Hernández Vega',   '1234567-8', 1, '44441001', 'luis.hernandez@email.com', NOW()),
(2, 'Sandra Patricia', 'Ramírez Castillo', '8765432-1', 0, '44441002', 'sandra.ramirez@email.com', NOW()),
(3, 'Diego Armando',   'Flores Archila',   '5678901-2', 1, '44441003', 'diego.flores@email.com',   NOW()),
(4, 'Carmen Rosa',     'Ajú Mendoza',      '3456789-0', 0, '44441004', 'carmen.aju@email.com',     NOW()),
(5, 'Pedro Antonio',   'Chávez Xicará',    '9012345-6', 1, '44441005', 'pedro.chavez@email.com',   NOW());

-- ============================================================
-- 5. PROVEEDORES  (sin dependencias)
-- ============================================================
INSERT IGNORE INTO proveedores
  (id_proveedor, proveedor, nit, direccion, telefono)
VALUES
(1, 'Distribuidora El Aguila',  '1122334-5', '6a Avenida, Zona 4, Guatemala',    '22221001'),
(2, 'Importadora Central S.A.', '5566778-9', 'Calzada Roosevelt, Mixco',         '22221002'),
(3, 'Alimentos del Sur Ltda.',  '9900112-3', 'Zona Industrial, Villa Nueva',     '22221003'),
(4, 'Comercial Pacífico',       '3344556-7', '12 Calle, Zona 1, Guatemala City', '22221004'),
(5, 'Grupo Logístico GT',       '7788990-1', 'Zona 12, Guatemala City',          '22221005');

-- ============================================================
-- 6. PRODUCTOS  (depende de marcas)
--    CORRECCIÓN: se eliminó la columna 'imagen' (no existe en la tabla)
-- ============================================================
INSERT IGNORE INTO productos
  (id_producto, producto, id_marca, descripcion,
   precio_costo, precio_venta, existencia, fecha_ingreso)
VALUES
(1, 'Leche Entera 1L',        1, 'Leche entera pasteurizada 1 litro',      12.50, 18.00, 200, NOW()),
(2, 'Coca-Cola 355ml',        2, 'Bebida gaseosa lata 355 ml',              8.00,  12.00, 500, NOW()),
(3, 'Colgate Triple Acción',  3, 'Pasta dental 90 ml protección triple',   15.00, 22.00, 150, NOW()),
(4, 'Jabón Dove 90g',         4, 'Jabón de tocador hidratante 90 gramos',   9.50,  14.00, 300, NOW()),
(5, 'Pan Bimbo Integral',     5, 'Pan de caja integral 580 g',             18.00, 25.00, 120, NOW()),
(6, 'Nescafé Clásico 100g',   1, 'Café soluble instantáneo frasco 100 g', 40.00, 58.00,  80, NOW()),
(7, 'Coca-Cola 2L',           2, 'Bebida gaseosa botella 2 litros',        16.00, 22.00, 250, NOW()),
(8, 'Enjuague Colgate 500ml', 3, 'Enjuague bucal menta fresca 500 ml',    28.00, 40.00,  90, NOW());

-- ============================================================
-- 7. VENTAS  (depende de clientes y empleados)
-- ============================================================
INSERT IGNORE INTO ventas
  (id_venta, no_factura, serie, fecha_factura, id_cliente, id_empleado, fecha_ingreso)
VALUES
(1, 10001, 'A', '2025-05-01', 1, 2, NOW()),
(2, 10002, 'A', '2025-05-03', 2, 2, NOW()),
(3, 10003, 'B', '2025-05-10', 3, 4, NOW()),
(4, 10004, 'B', '2025-05-15', 4, 4, NOW()),
(5, 10005, 'A', '2025-05-20', 5, 2, NOW());

-- ============================================================
-- 8. VENTAS_DETALLE  (depende de ventas y productos)
-- ============================================================
INSERT IGNORE INTO ventas_detalle
  (id_venta_detalle, id_venta, id_producto, cantidad, precio_unitario)
VALUES
(1,  1, 1, 3,  18.00),   -- Factura 10001: 3 Leche Entera
(2,  1, 2, 6,  12.00),   -- Factura 10001: 6 Coca-Cola 355ml
(3,  2, 3, 2,  22.00),   -- Factura 10002: 2 Colgate Triple
(4,  2, 4, 4,  14.00),   -- Factura 10002: 4 Jabón Dove
(5,  3, 5, 1,  25.00),   -- Factura 10003: 1 Pan Integral
(6,  3, 6, 2,  58.00),   -- Factura 10003: 2 Nescafé
(7,  4, 7, 5,  22.00),   -- Factura 10004: 5 Coca-Cola 2L
(8,  4, 8, 1,  40.00),   -- Factura 10004: 1 Enjuague Colgate
(9,  5, 1, 4,  18.00),   -- Factura 10005: 4 Leche Entera
(10, 5, 7, 2,  22.00);   -- Factura 10005: 2 Coca-Cola 2L

-- ============================================================
-- 9. COMPRAS  (depende de proveedores)
-- ============================================================
INSERT IGNORE INTO compras
  (id_compra, no_orden_compra, id_proveedor, fecha_orden, fecha_ingreso)
VALUES
(1, 20250001, 1, '2025-04-25', NOW()),
(2, 20250002, 2, '2025-04-28', NOW()),
(3, 20250003, 3, '2025-05-05', NOW()),
(4, 20250004, 4, '2025-05-12', NOW()),
(5, 20250005, 5, '2025-05-18', NOW());

-- ============================================================
-- 10. COMPRAS_DETALLE  (depende de compras y productos)
-- ============================================================
INSERT IGNORE INTO compras_detalle
  (id_compra_detalle, id_compra, id_producto, cantidad, precio_costo_unitario)
VALUES
(1,  1, 1, 100, 12.50),   -- OC-001: 100 Leche Entera
(2,  1, 6,  50, 40.00),   -- OC-001:  50 Nescafé
(3,  2, 2, 200,  8.00),   -- OC-002: 200 Coca-Cola 355ml
(4,  2, 7, 100, 16.00),   -- OC-002: 100 Coca-Cola 2L
(5,  3, 3,  80, 15.00),   -- OC-003:  80 Colgate Triple
(6,  3, 8,  60, 28.00),   -- OC-003:  60 Enjuague Colgate
(7,  4, 4, 150,  9.50),   -- OC-004: 150 Jabón Dove
(8,  4, 5,  60, 18.00),   -- OC-004:  60 Pan Integral
(9,  5, 1,  50, 12.50),   -- OC-005:  50 Leche Entera (reposición)
(10, 5, 6,  30, 40.00);   -- OC-005:  30 Nescafé (reposición)

-- ============================================================
--  FIN DEL SCRIPT
-- ============================================================
