#import "template.typ":*
#import "@preview/codelst:1.0.0": sourcecode

#let codelst-sourcecode = sourcecode
#let sourcecode = codelst-sourcecode.with(
frame: block.with(
fill: none,
stroke: none,
radius: 2pt,
inset: (x: 10pt, y: 5pt)
)
)

#show: project.with(
  title: [
    Курсова Робота\
    з дисципліни\
    Паралельні обчислення
  ],
  author: "Зарицький Кирило Андрійович"
)

#outline(
  title: [Зміст]
)

#show heading.where(
  level: 1
): it => [
  #pagebreak(weak: true)
  #it.body
]

#pagebreak()
= Вступ

#[



]

= Теоретичні Відомості

#[



]

= Модель системи

#[



]

= Опис реалізації

#[


  
]

= Виконання програми

#[

]

= Висновки

#[

]


