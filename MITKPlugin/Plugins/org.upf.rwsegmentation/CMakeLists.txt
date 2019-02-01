project(org_upf_rwsegmentation)

mitk_create_plugin(
  EXPORT_DIRECTIVE RWSEGMENTATION_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt
)
