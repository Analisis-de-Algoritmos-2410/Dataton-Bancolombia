# Necesita el output de los horarios (Las filas de 0s y 1s) como input para generar el excel
# Tener en cuenta que las franjas de 6 horarios con pausa activa son Almuerzo
import xlsxwriter


workbook = xlsxwriter.Workbook('dataton.xlsx')
worksheet = workbook.add_worksheet()

worksheet.write('A1', 'fecha_hora')
for i in range(1, 9, 1):
  worksheet.write(0, i, str(i))

H = []
for i in range(8):
  H.append(input())

row = 1
for h in H:
  col = 1
  for c in h:
    if c == '0':
      worksheet.write(col, row, "Pausa activa")
    else:
      worksheet.write(col, row, "Trabajando")
    col += 1
  row += 1

workbook.close()
