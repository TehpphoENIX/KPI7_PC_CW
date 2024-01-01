// The project function defines how your document looks.
// It takes your content and some metadata and formats it.
// Go ahead and customize it to your liking!
#let project(title: "", author: "", body) = {
  // Set the document's basic properties.
  set document(author: author, title: title)
  set page(
    margin: (left: 20mm, right: 10mm, top: 20mm, bottom: 20mm)
  )
  set text(font: "FreeSerif", lang: "uk",size: 14pt)

  // Title page.
  align(center,strong(
    "КПІ ім. Ігоря Сікорського\n"+
		"Інститут прикладного системного аналізу\n"+
		"Кафедра Системного проектування\n"
  ))
  v(8cm)
  // Title
  align(center,strong(title))
  // Author information.
  v(4cm)
  align(right,
    "Виконав:\n" +
		"Студент групи ДА-01\n" +
		"ННК «ІПСА»\n" +
		author + "\n" +
		"Варіант № 9")
  
  place(bottom + center, "Київ — 2023")

  pagebreak()

  // Main body.
  set page(numbering: "1", number-align: center)
  show heading: set text(size: 14pt)
  show heading: set block(above: 1.5em, below: 1.5em)
  set par(leading: 1.5em, justify: true)

  counter(page).update(1)

  body
}

#let LabWorkProject(
  labNumber: "number", 
  labSubject: "subject", 
  labName: "name", 
  author: "", body) = {
    project(
      title: "Лабораторна робота №" + labNumber + "\n"+
    		"з дисципліни «" + labSubject + "»\n"+
    		"на тему «" + labName + "»",
      author: author,
      body)
  }