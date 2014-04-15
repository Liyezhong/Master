/****************************************************************************/
/*! \file Helper.h
 *
 *  \brief Definition file for general purpose functions.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-01-21
 *  $Author:    $ R. Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATAMANAGER_HELPER_H
#define DATAMANAGER_HELPER_H

#include<QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace DataManager {

/****************************************************************************/
/**
 * \brief Helper class, which provides some static methods for general use.
 */
/****************************************************************************/

class XmlStartElementReaderHelper
{
public:
    XmlStartElementReaderHelper(QXmlStreamReader &reader);

    bool IsValid(void);

private:
    bool m_NonEnd;
};

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class XmlElementReaderHelper
 */
/****************************************************************************/
class XmlElementReaderHelper
{
public:
    XmlElementReaderHelper(QXmlStreamReader &reader, bool *hasChildren = 0);
    ~XmlElementReaderHelper(void);

    bool IsEnd(void);

private:
    QXmlStreamReader &m_Reader;
    bool             *m_HasChildren;
    bool              m_IsEnd;
};

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class XmlElementReaderHelper
 */
/****************************************************************************/
class XmlElementWriterHelper
{
public:
    XmlElementWriterHelper(QXmlStreamWriter &writer, const QString &elementName);

    ~XmlElementWriterHelper(void);

private:
    QXmlStreamWriter &m_Writer;
};

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class XmlElementReaderHelper
 */
/****************************************************************************/
class XmlDocumentWriterHelper
{
public:
    XmlDocumentWriterHelper(QXmlStreamWriter &writer);

    ~XmlDocumentWriterHelper(void);

private:
    QXmlStreamWriter &m_Writer;
};

typedef enum
{
    WRONG_NAME = -256,
    WRONG_ATTRIBUTE,
    END = -1,
    SUCCEED,
    HAS_CHILDREN
} XmlReadErrorType_t;

XmlReadErrorType_t ReadAttributesOfElement(const QString     &elementName,
                                           const QStringList &attributeNameList,
                                           QXmlStreamReader  &reader,
                                           QStringList       &attributeList);

XmlReadErrorType_t ReadAttributesOfElement(const QString     &elementName,
                                           QXmlStreamReader  &reader,
                                           QStringList       &attributeNameList,
                                           QStringList       &attributeList);

void WriteElementWithAttributes(const QString     &elementName,
                                const QStringList &attributeNameList,
                                const QStringList &attributeList,
                                QXmlStreamWriter  &writer);

} // namespace DataManager

#endif // DATAMANAGER_HELPER_H
