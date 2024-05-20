include(CSharpUtilities)

set(SRC
  App.config
  Demo.cst

  Demo.cs
  DemoComponent.cs

  ./CstCore/CstApplication.cs
  ./CstCore/CstNode.cs
  ./CstCore/CstComponent.cs
  ./CstCore/CstText.cs
  ./CstCore/CstWidget.cs
)

add_executable(CstMono "${SRC}")
